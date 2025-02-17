#pragma once

#ifdef _DEBUG
//Writes all compile/parse errors/warnings to a file. (0=never, 1=only errors, 2=warnings, 3=info)
#define DEBUG_LEVEL_LOG 1
//Writes all shaders that are compiled to separate files (e.g. ShaderName_Technique_Pass.vs and .fs) (0=never, 1=only if compile failed, 2=always)
#define WRITE_SHADER_FILES 1
#else 
#define DEBUG_LEVEL_LOG 0
#define WRITE_SHADER_FILES 1
#endif

// Attempt to speed up STL which is very CPU costly, maybe we should look into using EASTL instead? http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html https://github.com/electronicarts/EASTL
#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0

#ifdef ENABLE_SDL
#include <map>
#include <string>
#endif

#if defined(ENABLE_SDL) && defined(TWEAK_GL_SHADER)
//Todo tweak Enums for uniforms and techniques to reuse same numbers in different shaders/techniques. Reduces the array sizes, but might be hard to debug.
enum shaderUniforms {
   //Floats
   SHADER_blend_modulate_vs_add, SHADER_alphaTestValue, SHADER_flasherMode, SHADER_eye, SHADER_fKickerScale,
   //Vectors and Float Arrays
   SHADER_Roughness_WrapL_Edge_Thickness, SHADER_cBase_Alpha, SHADER_lightCenter_maxRange, SHADER_lightColor2_falloff_power, SHADER_lightColor_intensity, SHADER_matrixBlock, SHADER_fenvEmissionScale_TexWidth,
   SHADER_invTableRes_playfield_height_reflection, SHADER_lightEmission, SHADER_lightPos, SHADER_orientation, SHADER_cAmbient_LightRange, SHADER_cClearcoat_EdgeAlpha, SHADER_cGlossy_ImageLerp,
   SHADER_fDisableLighting_top_below, SHADER_backBoxSize, SHADER_quadOffsetScale, SHADER_quadOffsetScaleTex, SHADER_vColor_Intensity, SHADER_w_h_height, SHADER_alphaTestValueAB_filterMode_addBlend,
   SHADER_amount_blend_modulate_vs_add_hdrTexture01, SHADER_staticColor_Alpha, SHADER_width_height_rotated_flipLR, SHADER_vRes_Alpha_time, SHADER_mirrorFactor, SHADER_SSR_bumpHeight_fresnelRefl_scale_FS, SHADER_AO_scale_timeblur,
   //Integer and Bool
   SHADER_ignoreStereo, SHADER_SRGBTexture, SHADER_hdrTexture0, SHADER_disableLighting, SHADER_lightSources, SHADER_doNormalMapping, SHADER_hdrEnvTextures, SHADER_is_metal, SHADER_color_grade, SHADER_do_bloom, SHADER_lightingOff, SHADER_objectSpaceNormalMap, SHADER_do_dither,
   //Textures
   SHADER_Texture0, SHADER_Texture1, SHADER_Texture2, SHADER_Texture3, SHADER_Texture4, SHADER_edgesTex2D, SHADER_blendTex2D, SHADER_areaTex2D, SHADER_searchTex2D,
   SHADER_UNIFORM_COUNT, SHADER_UNIFORM_INVALID
};

enum shaderAttributes {
   SHADER_ATTRIBUTE_POS, SHADER_ATTRIBUTE_NORM, SHADER_ATTRIBUTE_TC, SHADER_ATTRIBUTE_TEX,
   SHADER_ATTRIBUTE_COUNT, SHADER_ATTRIBUTE_INVALID
};

enum shaderTechniques {
   SHADER_TECHNIQUE_RenderBall, SHADER_TECHNIQUE_RenderBall_DecalMode, SHADER_TECHNIQUE_RenderBall_CabMode, SHADER_TECHNIQUE_RenderBall_CabMode_DecalMode, SHADER_TECHNIQUE_RenderBallTrail,
   SHADER_TECHNIQUE_basic_without_texture, SHADER_TECHNIQUE_basic_with_texture, SHADER_TECHNIQUE_basic_depth_only_without_texture, SHADER_TECHNIQUE_basic_depth_only_with_texture, SHADER_TECHNIQUE_bg_decal_without_texture,
   SHADER_TECHNIQUE_bg_decal_with_texture, SHADER_TECHNIQUE_kickerBoolean, SHADER_TECHNIQUE_light_with_texture, SHADER_TECHNIQUE_light_without_texture,
   SHADER_TECHNIQUE_basic_DMD, SHADER_TECHNIQUE_basic_DMD_ext, SHADER_TECHNIQUE_basic_DMD_world, SHADER_TECHNIQUE_basic_DMD_world_ext, SHADER_TECHNIQUE_basic_noDMD, SHADER_TECHNIQUE_basic_noDMD_world, SHADER_TECHNIQUE_basic_noDMD_notex,
   SHADER_TECHNIQUE_AO, SHADER_TECHNIQUE_NFAA, SHADER_TECHNIQUE_DLAA_edge, SHADER_TECHNIQUE_DLAA, SHADER_TECHNIQUE_FXAA1, SHADER_TECHNIQUE_FXAA2, SHADER_TECHNIQUE_FXAA3, SHADER_TECHNIQUE_fb_tonemap, SHADER_TECHNIQUE_fb_bloom,
   SHADER_TECHNIQUE_fb_AO, SHADER_TECHNIQUE_fb_tonemap_AO, SHADER_TECHNIQUE_fb_tonemap_AO_static, SHADER_TECHNIQUE_fb_tonemap_no_filterRGB, SHADER_TECHNIQUE_fb_tonemap_no_filterRG, SHADER_TECHNIQUE_fb_tonemap_no_filterR, 
   SHADER_TECHNIQUE_fb_tonemap_AO_no_filter, SHADER_TECHNIQUE_fb_tonemap_AO_no_filter_static, SHADER_TECHNIQUE_fb_bloom_horiz9x9, SHADER_TECHNIQUE_fb_bloom_vert9x9, SHADER_TECHNIQUE_fb_bloom_horiz19x19, SHADER_TECHNIQUE_fb_bloom_vert19x19,
   SHADER_TECHNIQUE_fb_bloom_horiz19x19h, SHADER_TECHNIQUE_fb_bloom_vert19x19h, SHADER_TECHNIQUE_SSReflection, SHADER_TECHNIQUE_fb_mirror, SHADER_TECHNIQUE_basic_noLight, SHADER_TECHNIQUE_bulb_light,
   SHADER_TECHNIQUE_SMAA_ColorEdgeDetection, SHADER_TECHNIQUE_SMAA_BlendWeightCalculation, SHADER_TECHNIQUE_SMAA_NeighborhoodBlending,
   SHADER_TECHNIQUE_stereo_TB, SHADER_TECHNIQUE_stereo_SBS, SHADER_TECHNIQUE_stereo_Int, SHADER_TECHNIQUE_stereo_AMD_DEBUG,
   SHADER_TECHNIQUE_COUNT, SHADER_TECHNIQUE_INVALID
};

typedef shaderUniforms SHADER_UNIFORM_HANDLE;
typedef shaderTechniques SHADER_TECHNIQUE_HANDLE;
typedef void ID3DXEffect;

#else

//Float
#define SHADER_blend_modulate_vs_add "blend_modulate_vs_add"
#define SHADER_alphaTestValue "alphaTestValue"
#define SHADER_flasherMode "flasherMode"
#define SHADER_eye "eye"
#define SHADER_fKickerScale "fKickerScale"

//Vectors and Float Arrays
#define SHADER_Roughness_WrapL_Edge_Thickness "Roughness_WrapL_Edge_Thickness"
#define SHADER_cBase_Alpha "cBase_Alpha"
#define SHADER_lightCenter_maxRange "lightCenter_maxRange"
#define SHADER_lightColor2_falloff_power "lightColor2_falloff_power"
#define SHADER_lightColor_intensity "lightColor_intensity"
#define SHADER_matrixBlock "matrixBlock"
#define SHADER_fenvEmissionScale_TexWidth "fenvEmissionScale_TexWidth"
#define SHADER_invTableRes_playfield_height_reflection "invTableRes__playfield_height_reflection"
#define SHADER_lightEmission "lightEmission"
#define SHADER_lightPos "lightPos"
#define SHADER_orientation "orientation"
#define SHADER_cAmbient_LightRange "cAmbient_LightRange"
#define SHADER_cClearcoat_EdgeAlpha "cClearcoat_EdgeAlpha"
#define SHADER_cGlossy_ImageLerp "cGlossy_ImageLerp"
#define SHADER_fDisableLighting_top_below "fDisableLighting_top_below"
#define SHADER_backBoxSize "backBoxSize"
#define SHADER_quadOffsetScale "quadOffsetScale"
#define SHADER_quadOffsetScaleTex "quadOffsetScaleTex"
#define SHADER_vColor_Intensity "vColor_Intensity"
#define SHADER_w_h_height "w_h_height"
#define SHADER_alphaTestValueAB_filterMode_addBlend "alphaTestValueAB_filterMode_addBlend"
#define SHADER_amount_blend_modulate_vs_add_hdrTexture01 "amount__blend_modulate_vs_add__hdrTexture01"
#define SHADER_staticColor_Alpha "staticColor_Alpha"
#define SHADER_width_height_rotated_flipLR "width_height_rotated_flipLR"
#define SHADER_vRes_Alpha_time "vRes_Alpha_time"
#define SHADER_mirrorFactor "mirrorFactor"
#define SHADER_SSR_bumpHeight_fresnelRefl_scale_FS "SSR_bumpHeight_fresnelRefl_scale_FS"
#define SHADER_AO_scale_timeblur "AO_scale_timeblur"

//Integer
#define SHADER_ignoreStereo "ignoreStereo"
#define SHADER_SRGBTexture "SRGBTexture"
#define SHADER_hdrTexture0 "hdrTexture0"
#define SHADER_disableLighting "disableLighting"
#define SHADER_lightSources "lightSources"
#define SHADER_doNormalMapping "doNormalMapping"
#define SHADER_hdrEnvTextures "hdrEnvTextures"
#define SHADER_is_metal "is_metal"
#define SHADER_color_grade "color_grade"
#define SHADER_do_bloom "do_bloom"
#define SHADER_lightingOff "lightingOff"
#define SHADER_objectSpaceNormalMap "objectSpaceNormalMap"
#define SHADER_do_dither "do_dither"

//Textures
#define SHADER_Texture0 "Texture0"
#define SHADER_Texture1 "Texture1"
#define SHADER_Texture2 "Texture2"
#define SHADER_Texture3 "Texture3"
#define SHADER_Texture4 "Texture4"
#define SHADER_edgesTex2D "edgesTex2D"
#define SHADER_blendTex2D "blendTex2D"
#define SHADER_areaTex2D "areaTex2D"
#define SHADER_searchTex2D "searchTex2D"

//Attributes
#define SHADER_ATTRIBUTE_POS "vPosition"
#define SHADER_ATTRIBUTE_NORM "vNormal"
#define SHADER_ATTRIBUTE_TC "tc"
#define SHADER_ATTRIBUTE_TEX "tex0"

//Shader Techniques
#define SHADER_TECHNIQUE_RenderBall "RenderBall"
#define SHADER_TECHNIQUE_RenderBall_DecalMode "RenderBall_DecalMode"
#define SHADER_TECHNIQUE_RenderBall_CabMode "RenderBall_CabMode"
#define SHADER_TECHNIQUE_RenderBall_CabMode_DecalMode "RenderBall_CabMode_DecalMode"
#define SHADER_TECHNIQUE_RenderBallTrail "RenderBallTrail"

#define SHADER_TECHNIQUE_basic_without_texture "basic_without_texture"
#define SHADER_TECHNIQUE_basic_with_texture "basic_with_texture"
#define SHADER_TECHNIQUE_basic_depth_only_without_texture "basic_depth_only_without_texture"
#define SHADER_TECHNIQUE_basic_depth_only_with_texture "basic_depth_only_with_texture"
#define SHADER_TECHNIQUE_bg_decal_without_texture "bg_decal_without_texture"
#define SHADER_TECHNIQUE_bg_decal_with_texture "bg_decal_with_texture"
#define SHADER_TECHNIQUE_kickerBoolean "kickerBoolean"
#define SHADER_TECHNIQUE_light_with_texture "light_with_texture"
#define SHADER_TECHNIQUE_light_without_texture "light_without_texture"

#define SHADER_TECHNIQUE_basic_DMD "basic_DMD"
#define SHADER_TECHNIQUE_basic_DMD_ext "basic_DMD_ext"
#define SHADER_TECHNIQUE_basic_DMD_world "basic_DMD_world"
#define SHADER_TECHNIQUE_basic_DMD_world_ext "basic_DMD_world_ext"
#define SHADER_TECHNIQUE_basic_noDMD "basic_noDMD"
#define SHADER_TECHNIQUE_basic_noDMD_world "basic_noDMD_world"
#define SHADER_TECHNIQUE_basic_noDMD_notex "basic_noDMD_notex"

#define SHADER_TECHNIQUE_AO "AO"
#define SHADER_TECHNIQUE_NFAA "NFAA"
#define SHADER_TECHNIQUE_DLAA_edge "DLAA_edge"
#define SHADER_TECHNIQUE_DLAA "DLAA"
#define SHADER_TECHNIQUE_FXAA1 "FXAA1"
#define SHADER_TECHNIQUE_FXAA2 "FXAA2"
#define SHADER_TECHNIQUE_FXAA3 "FXAA3"
#define SHADER_TECHNIQUE_fb_tonemap "fb_tonemap"
#define SHADER_TECHNIQUE_fb_bloom "fb_bloom"
#define SHADER_TECHNIQUE_fb_AO "fb_AO"
#define SHADER_TECHNIQUE_fb_tonemap_AO "fb_tonemap_AO"
#define SHADER_TECHNIQUE_fb_tonemap_AO_static "fb_tonemap_AO_static"
#define SHADER_TECHNIQUE_fb_tonemap_no_filterRGB "fb_tonemap_no_filterRGB"
#define SHADER_TECHNIQUE_fb_tonemap_no_filterRG "fb_tonemap_no_filterRG"
#define SHADER_TECHNIQUE_fb_tonemap_no_filterR "fb_tonemap_no_filterR"
#define SHADER_TECHNIQUE_fb_tonemap_AO_no_filter "fb_tonemap_AO_no_filter"
#define SHADER_TECHNIQUE_fb_tonemap_AO_no_filter_static "fb_tonemap_AO_no_filter_static"
#define SHADER_TECHNIQUE_fb_bloom_horiz9x9 "fb_bloom_horiz9x9"
#define SHADER_TECHNIQUE_fb_bloom_vert9x9 "fb_bloom_vert9x9"
#define SHADER_TECHNIQUE_fb_bloom_horiz19x19 "fb_bloom_horiz19x19"
#define SHADER_TECHNIQUE_fb_bloom_vert19x19 "fb_bloom_vert19x19"
#define SHADER_TECHNIQUE_fb_bloom_horiz19x19h "fb_bloom_horiz19x19h"
#define SHADER_TECHNIQUE_fb_bloom_vert19x19h "fb_bloom_vert19x19h"
#define SHADER_TECHNIQUE_SSReflection "SSReflection"
#define SHADER_TECHNIQUE_fb_mirror "fb_mirror"

#define SHADER_TECHNIQUE_basic_noLight "basic_noLight"

#define SHADER_TECHNIQUE_bulb_light "bulb_light"

#define SHADER_TECHNIQUE_SMAA_ColorEdgeDetection "SMAA_ColorEdgeDetection"
#define SHADER_TECHNIQUE_SMAA_BlendWeightCalculation "SMAA_BlendWeightCalculation"
#define SHADER_TECHNIQUE_SMAA_NeighborhoodBlending "SMAA_NeighborhoodBlending"

#define SHADER_TECHNIQUE_stereo_TB "stereo_TB"
#define SHADER_TECHNIQUE_stereo_SBS "stereo_SBS"
#define SHADER_TECHNIQUE_stereo_Int "stereo_Int"
#define SHADER_TECHNIQUE_stereo_AMD_DEBUG "stereo_AMD_DEBUG"

#ifdef ENABLE_SDL
typedef char* SHADER_UNIFORM_HANDLE;
typedef char* SHADER_TECHNIQUE_HANDLE;
typedef void ID3DXEffect;
#endif
#endif

#ifndef ENABLE_SDL
typedef D3DXHANDLE SHADER_UNIFORM_HANDLE;
typedef D3DXHANDLE SHADER_TECHNIQUE_HANDLE;
#endif

class Shader
{
public:
   Shader(RenderDevice *renderDevice);
   ~Shader();

#ifdef ENABLE_SDL
   bool Load(const char* shaderCodeName, UINT codeSize);
#else
   bool Load(const BYTE* shaderCodeName, UINT codeSize);
#endif
   void Unload();

   void Begin(const unsigned int pass);
   void End();

   void SetTexture(const SHADER_UNIFORM_HANDLE texelName, Texture *texel, const bool linearRGB, const bool clamptoedge = false);
   void SetTexture(const SHADER_UNIFORM_HANDLE texelName, D3DTexture *texel, const bool linearRGB);
   void SetTextureDepth(const SHADER_UNIFORM_HANDLE texelName, D3DTexture *texel);
   void SetTextureNull(const SHADER_UNIFORM_HANDLE texelName);
   void SetMaterial(const Material * const mat);

   void SetDisableLighting(const float value); // only set top
   void SetDisableLighting(const vec4& value); // set top and below
   void SetAlphaTestValue(const float value);
   void SetFlasherColorAlpha(const vec4& color);
   void SetFlasherData(const vec4& color, const float mode);
   void SetLightColorIntensity(const vec4& color);
   void SetLightColor2FalloffPower(const vec4& color);
   void SetLightData(const vec4& color);
   void SetLightImageBackglassMode(const bool imageMode, const bool backglassMode);

   void SetTechnique(const SHADER_TECHNIQUE_HANDLE technique);
   void SetTechniqueMetal(const SHADER_TECHNIQUE_HANDLE technique, const bool isMetal);

   void SetMatrix(const SHADER_UNIFORM_HANDLE hParameter, const Matrix3D* pMatrix);
   void SetUniformBlock(const SHADER_UNIFORM_HANDLE hParameter, const float* pMatrix, const size_t size);
   void SetVector(const SHADER_UNIFORM_HANDLE hParameter, const vec4* pVector);
   void SetVector(const SHADER_UNIFORM_HANDLE hParameter, const float x, const float y, const float z, const float w);
   void SetFloat(const SHADER_UNIFORM_HANDLE hParameter, const float f);
   void SetInt(const SHADER_UNIFORM_HANDLE hParameter, const int i);
   void SetBool(const SHADER_UNIFORM_HANDLE hParameter, const bool b);
   void SetFloatArray(const SHADER_UNIFORM_HANDLE hParameter, const float* pData, const unsigned int count);

   static void SetTransform(const TransformStateType p1, const Matrix3D* p2, const int count);
   static void GetTransform(const TransformStateType p1, Matrix3D* p2, const int count);
   static Shader* getCurrentShader();

private:
   static Shader* m_currentShader;
   static RenderDevice *m_renderDevice;
   static int shaderCount;

   // caches:
   Material currentMaterial;

   vec4 currentDisableLighting; // x and y: top and below, z and w unused

   static constexpr DWORD TEXTURESET_STATE_CACHE_SIZE = 5; // current convention: SetTexture gets "TextureX", where X 0..4
   BaseTexture *currentTexture[TEXTURESET_STATE_CACHE_SIZE];
   float currentAlphaTestValue;
   char  currentTechnique[64];

   vec4  currentFlasherColor; // flasher only-data
   vec4  currentFlasherData;
   float currentFlasherMode;

   vec4 currentLightColor; // all light only-data
   vec4 currentLightColor2;
   vec4 currentLightData;
   unsigned int currentLightImageMode;
   unsigned int currentLightBackglassMode;

#ifdef ENABLE_SDL
   const char* m_shaderCodeName = nullptr; // Only valid while loading

#if DEBUG_LEVEL_LOG > 0
   void LOG(const int level, const char* fileNameRoot, const string& message);
#endif
   bool parseFile(const char* fileNameRoot, const char* fileName, int level, std::map<string, string>& values, const string& parentMode);
   string analyzeFunction(const char* shaderCodeName, const string& technique, const string& functionName, const std::map<string, string>& values);
   bool compileGLShader(const char* fileNameRoot, const string& shaderCodeName, const string& vertex, const string& geometry, const string& fragment);

   struct attributeLoc {
      GLenum type;
      int location;
      int size;
   };
   struct uniformLoc {
      GLenum type;
      int location;
      int size;
      GLuint blockBuffer;
   };
   struct floatP {
      size_t len;
      float* data;
   };

#ifdef TWEAK_GL_SHADER
   struct glShader {
      int program;
      attributeLoc attributeLocation[SHADER_ATTRIBUTE_COUNT];
      uniformLoc uniformLocation[SHADER_UNIFORM_COUNT];
   };
   glShader shaderList[SHADER_TECHNIQUE_COUNT];
   float uniformFloat[SHADER_UNIFORM_COUNT];
   floatP uniformFloatP[SHADER_UNIFORM_COUNT];
   int uniformInt[SHADER_UNIFORM_COUNT];
   int uniformTex[SHADER_UNIFORM_COUNT];
   shaderTechniques technique;
   shaderUniforms getUniformByName(const string& name);
   shaderAttributes getAttributeByName(const string& name);
   shaderTechniques getTechniqueByName(const string& name);

#else

   struct glShader {
      int program;
      std::map<string, attributeLoc> *attributeLocation;
      std::map<string, uniformLoc> *uniformLocation;
   };
   std::map<string, glShader> shaderList;
   std::map<string, float> uniformFloat;
   std::map<string, floatP> uniformFloatP;
   std::map<string, int> uniformInt;
   std::map<string, int> uniformTex;
   char technique[256];
#endif

   static Matrix3D mWorld, mView, mProj[2];
   static int lastShaderProgram;
   static D3DTexture* noTexture;
   static D3DTexture* noTextureMSAA;
   static float* zeroData;
   static int nextTextureSlot;
   static int* textureSlotList;
   static std::map<int, int> slotTextureList;
   static int maxSlots;

   glShader* m_currentTechnique;

public:
   void setAttributeFormat(const DWORD fvf);

   static void setTextureDirty(const int TextureID);
   static std::string shaderPath;
   static std::string Defines;

#else

   ID3DXEffect * m_shader;

#endif
};
