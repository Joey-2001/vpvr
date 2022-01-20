#include "stdafx.h"
#include "VertexBuffer.h"
#include "RenderDevice.h"
#include "Shader.h"

// Disabled since it still has some bugs
#define COMBINE_BUFFERS 0

static unsigned int fvfToSize(const DWORD fvf)
{
   switch (fvf)
   {
   case MY_D3DFVF_NOTEX2_VERTEX:
   case MY_D3DTRANSFORMED_NOTEX2_VERTEX:
      return sizeof(Vertex3D_NoTex2);
   case MY_D3DFVF_TEX:
      return sizeof(Vertex3D_TexelOnly);
   default:
      return 0;
   }
}

VertexBuffer* VertexBuffer::m_curVertexBuffer = nullptr;

#ifndef ENABLE_SDL
IDirect3DDevice9* VertexBuffer::m_pD3DDevice = nullptr;
#endif

#ifdef ENABLE_SDL
std::vector<VertexBuffer*> VertexBuffer::notUploadedBuffers;
#endif

void VertexBuffer::CreateVertexBuffer(const unsigned int vertexCount, const DWORD usage, const DWORD fvf, VertexBuffer **vBuffer)
{
#ifdef ENABLE_SDL
   VertexBuffer* const vb = new VertexBuffer();
   vb->count = vertexCount;
   vb->sizePerVertex = fvfToSize(fvf);
   vb->usage = usage ? usage : USAGE_STATIC;
   vb->fvf = fvf;
   *vBuffer = vb;
   vb->isUploaded = false;
   vb->size = vb->sizePerVertex * vb->count;
   vb->Array = 0;
   vb->Buffer = 0;
#else
   // NB: We always specify WRITEONLY since MSDN states,
   // "Buffers created with D3DPOOL_DEFAULT that do not specify D3DUSAGE_WRITEONLY may suffer a severe performance penalty."
   // This means we cannot read from vertex buffers, but I don't think we need to.
   const HRESULT hr = m_pD3DDevice->CreateVertexBuffer(vertexCount * fvfToSize(fvf), D3DUSAGE_WRITEONLY | usage, 0,
      (D3DPOOL)memoryPool::DEFAULT, (IDirect3DVertexBuffer9**)vBuffer, nullptr);
   if (FAILED(hr))
      ReportError("Fatal Error: unable to create vertex buffer!", hr, __FILE__, __LINE__);
   (*vBuffer)->m_fvf = fvf;
#endif
}

void VertexBuffer::lock(const unsigned int offsetToLock, const unsigned int sizeToLock, void **dataBuffer, const DWORD flags)
{
   m_curLockCalls++;
#ifdef ENABLE_SDL
   if (sizeToLock == 0)
      this->sizeToLock = size;
   else
      this->sizeToLock = sizeToLock;
   if (offsetToLock < size) {
      *dataBuffer = malloc(this->sizeToLock);
      this->dataBuffer = *dataBuffer;
      this->offsetToLock = offsetToLock;
   }
   else {
      *dataBuffer = nullptr;
      this->dataBuffer = nullptr;
      this->sizeToLock = 0;
   }
#else
   CHECKD3D(this->Lock(offsetToLock, sizeToLock, dataBuffer, flags));
#endif
}

void VertexBuffer::unlock()
{
#ifdef ENABLE_SDL
   if (!dataBuffer)
      return;
   addToNotUploadedBuffers();
#else
   CHECKD3D(this->Unlock());
#endif
}

void VertexBuffer::release()
{
#ifdef ENABLE_SDL
   if (!sharedBuffer && Buffer != 0) {
      CHECKD3D(glDeleteBuffers(1, &Buffer));
      Buffer = 0;
      sizePerVertex = 0;
      offset = 0;
      count = 0;
      size = 0;
   }
#else
   SAFE_RELEASE_NO_CHECK_NO_SET(this);
#endif
}

void VertexBuffer::bind()
{
#ifdef ENABLE_SDL
   if (!isUploaded) {
      if (sharedBuffer)
         UploadBuffers();
      else
         UploadData();
   }
   if (m_curVertexBuffer == nullptr || this->Array != m_curVertexBuffer->Array || this->Buffer != m_curVertexBuffer->Buffer)
   {
      CHECKD3D(glBindVertexArray(this->Array));
      CHECKD3D(glBindBuffer(GL_ARRAY_BUFFER, this->Buffer));
      m_curVertexBuffer = this;
   }
   Shader::getCurrentShader()->setAttributeFormat(fvf);
#else
   if (m_curVertexBuffer == nullptr || m_curVertexBuffer != this)
   {
      const unsigned int vsize = fvfToSize(m_fvf);
      CHECKD3D(m_pD3DDevice->SetStreamSource(0, this, 0, vsize));
      m_curVertexBuffer = this;
   }
#endif
}

#ifdef ENABLE_SDL
void VertexBuffer::UploadData()
{
   if (Array == 0)
      glGenVertexArrays(1, &Array);
   CHECKD3D(glBindVertexArray(this->Array));
   if (Buffer == 0) {
      glGenBuffers(1, &Buffer);
      CHECKD3D(glBindBuffer(GL_ARRAY_BUFFER, this->Buffer));
      glBufferData(GL_ARRAY_BUFFER, size, nullptr, usage);
   }
   else
      CHECKD3D(glBindBuffer(GL_ARRAY_BUFFER, this->Buffer));
   if (size - offsetToLock > 0)
      CHECKD3D(glBufferSubData(GL_ARRAY_BUFFER, offset * fvfToSize(fvf) + offsetToLock, min(sizeToLock, size - offsetToLock), dataBuffer));
   CHECKD3D(glBindBuffer(GL_ARRAY_BUFFER, 0));
   CHECKD3D(glBindVertexArray(0));
   isUploaded = true;
   free(dataBuffer);
   dataBuffer = nullptr;
}

void VertexBuffer::addToNotUploadedBuffers()
{
   if (COMBINE_BUFFERS == 0 || usage != USAGE_STATIC)
      UploadData();
   else if (std::find(notUploadedBuffers.begin(), notUploadedBuffers.end(), this) == notUploadedBuffers.end())
      notUploadedBuffers.push_back(this);
}

void VertexBuffer::UploadBuffers()
{
   if (notUploadedBuffers.size() == 0) return;

   int countNT = 0;
   int countT = 0;
   GLuint BufferNT;
   GLuint BufferT;
   GLuint ArrayNT;
   GLuint ArrayT;
   glGenVertexArrays(1, &ArrayNT);
   glGenVertexArrays(1, &ArrayT);
   glGenBuffers(1, &BufferNT);
   glGenBuffers(1, &BufferT);
   for (auto it = notUploadedBuffers.begin(); it != notUploadedBuffers.end(); it++) {
      if (!(*it)->isUploaded && (*it)->usage == GL_STATIC_DRAW) {
         if ((*it)->fvf == MY_D3DFVF_TEX) {
            (*it)->offset = countT;
            countT += (*it)->count;
            (*it)->Buffer = BufferT;
            (*it)->Array = ArrayT;
         }
         else {
            (*it)->offset = countNT;
            countNT += (*it)->size;
            (*it)->Buffer = BufferNT;
            (*it)->Array = ArrayNT;
         }
         (*it)->sharedBuffer = true;
      }
   }
   //Allocate BufferData on GPU
   if (countNT > 0) {
      CHECKD3D(glBindBuffer(GL_ARRAY_BUFFER, BufferNT));
      CHECKD3D(glBindVertexArray(ArrayNT));
      CHECKD3D(glBufferData(GL_ARRAY_BUFFER, countNT * fvfToSize(MY_D3DFVF_NOTEX2_VERTEX), nullptr, GL_STATIC_DRAW));
   }
   if (countT > 0) {
      CHECKD3D(glBindBuffer(GL_ARRAY_BUFFER, BufferT));
      CHECKD3D(glBindVertexArray(ArrayT));
      CHECKD3D(glBufferData(GL_ARRAY_BUFFER, countT * fvfToSize(MY_D3DFVF_TEX), nullptr, GL_STATIC_DRAW));
   }
   for (auto it = notUploadedBuffers.begin(); it != notUploadedBuffers.end(); it++)
      (*it)->UploadData();
   CHECKD3D(glBindBuffer(GL_ARRAY_BUFFER, 0));
   CHECKD3D(glBindVertexArray(0));
   notUploadedBuffers.clear();
}
#endif
