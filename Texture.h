#pragma once

#define MIN_TEXTURE_SIZE 8

struct FIBITMAP;

// texture stored in main memory in 32bit ARGB uchar format or 96bit RGB float
class BaseTexture
{
public:
   enum Format
   {
      RGBA,
      RGB_FP
   };

   BaseTexture()
      : m_width(0), m_height(0), m_realWidth(0), m_realHeight(0), m_format(RGBA)
   { }

   BaseTexture(const int w, const int h, const Format format = RGBA)
      : m_width(w), m_height(h), m_data((format == RGBA ? 4 : 3 * 4) * (w*h)), m_realWidth(w), m_realHeight(h), m_format(format)
   { }

   int width() const { return m_width; }
   int height() const { return m_height; }
   int pitch() const { return (m_format == RGBA ? 4 : 3 * 4) * m_width; } // pitch in bytes
   BYTE* data() { return m_data.data(); }

private:
   int m_width;
   int m_height;

public:
   std::vector<BYTE> m_data;
   int m_realWidth, m_realHeight;
   Format m_format;

   void SetOpaque();

   void CopyFrom_Raw(const void* bits)  // copy bits which are already in the right format
   {
      memcpy(data(), bits, m_data.size());
   }

   void CopyTo_ConvertAlpha(BYTE* const bits); // premultiplies alpha (as Win32 AlphaBlend() wants it like that) OR converts rgb_fp format to 32bits


   static BaseTexture *CreateFromHBitmap(const HBITMAP hbm);
   static BaseTexture *CreateFromFile(const char *filename);
   static BaseTexture *CreateFromFreeImage(FIBITMAP* dib);
   static BaseTexture *CreateFromData(const void *data, const size_t size);
};

class Texture : public ILoadable
{
public:
   Texture();
   Texture(BaseTexture * const base);
   virtual ~Texture();

   // ILoadable callback
   virtual bool LoadToken(const int id, BiffReader * const pbr);

   HRESULT SaveToStream(IStream *pstream, PinTable *pt);
   HRESULT LoadFromStream(IStream *pstream, int version, PinTable *pt);

   void FreeStuff();

   void EnsureHBitmap();
   void CreateGDIVersion();

   void CreateTextureOffscreen(const int width, const int height);
   BaseTexture *CreateFromHBitmap(const HBITMAP hbm);
   void CreateFromResource(const int id);

   bool IsHDR() const
   {
      if (m_pdsBuffer == NULL)
         return false;
      else
         return (m_pdsBuffer->m_format == BaseTexture::RGB_FP);
   }

   void SetSizeFrom(const BaseTexture* const tex)
   {
      m_width = tex->width();
      m_height = tex->height();
      m_realWidth = tex->m_realWidth;
      m_realHeight = tex->m_realHeight;
   }

   // create/release a DC which contains a (read-only) copy of the texture; for editor use
   void GetTextureDC(HDC *pdc);
   void ReleaseTextureDC(HDC dc);

private:
   bool LoadFromMemory(BYTE * const data, const DWORD size);

public:

   // width and height of texture can be different than width and height
   // of m_pdsBuffer, since the surface can be limited to smaller sizes by the user
   int m_width, m_height;
   int m_realWidth, m_realHeight;
   float m_alphaTestValue;
   BaseTexture* m_pdsBuffer;

   HBITMAP m_hbmGDIVersion; // HBitmap at screen depth and converted/visualized alpha so GDI draws it fast
   PinBinary *m_ppb;  // if this image should be saved as a binary stream, otherwise just LZW compressed from the live bitmap

   char m_szName[MAXTOKEN];
   char m_szInternalName[MAXTOKEN];
   char m_szPath[MAX_PATH];

private:
   HBITMAP m_oldHBM;        // this is to cache the result of SelectObject()
};
