#pragma once
#ifndef ZP_FONT_H
#define ZP_FONT_H

struct zpGlyph
{
    zpRectf uvRect;
    zpVector2i size;
    zpVector2i bearing;
    zp_uint advance;
    zp_uint baseline;
};

struct zpFont
{
    zpMaterialHandle fontMaterial;
    zpGlyph fontGlyphs[ 256 ];
};

struct zpFontInstance
{
    zpString fontName;
    zp_size_t refCount;
    zp_hash64 instanceId;
    zp_time_t lastModifiedTime;
    zpFont font;
};

class zpFontHandle
{
public:
    zpFontHandle();
    zpFontHandle( const zpFontHandle& other );
    zpFontHandle( zpFontHandle&& other );
    ~zpFontHandle();

    zpFontHandle& operator=( const zpFontHandle& other );
    zpFontHandle& operator=( zpFontHandle&& other );

    const zpFont* operator->() const;
    zpFont* operator->();

    zp_bool isValid() const;
    void release();

private:
    void addRef();
    void releaseRef();

    void set( zp_hash64 instanceId, zpFontInstance* fontInstance );

    zpFontInstance* m_fontInstance;
    zp_hash64 m_instanceId;

    friend class zpFontManager;
};

class zpFontManager
{
public:
    zpFontManager();
    ~zpFontManager();

    void setup( zpMaterialManager* materialManager );
    void teardown();

    zp_bool loadFont( const zp_char* fontFile, zpFontHandle& font );
    zp_bool getFont( const zp_char* fontName, zpFontHandle& font );

    void garbageCollect();
    void reloadChangedFonts();


private:
    zpVector< zpFontInstance* > m_fontInstances;

    zpMaterialManager* m_materialManager;

    zp_hash64 m_newFontInstanceId;
};

#endif // !ZP_FONT_H
