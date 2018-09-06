#pragma once
#ifndef ZP_RENDER_COMMAND_BUFFER_H
#define ZP_RENDER_COMMAND_BUFFER_H

class zpRenderCommandBuffer
{
public:
    zpRenderCommandBuffer();
    zpRenderCommandBuffer( zp_size_t capacity );
    ~zpRenderCommandBuffer();

    void pushMarker( const zp_char* marker );
    void popMarker();

    void clearColorDepthStencil( const zpColorf& clearColor, zp_float clearDepth, zp_uint clearStencil );
    void clearColorDepth( const zpColorf& clearColor, zp_float clearDepth );
    void clearDepthStencil( zp_float clearDepth, zp_uint clearStencil );
    void clearColor( const zpColorf& clearColor );
    void clearDepth( zp_float clearDepth );
    void clearStencil( zp_uint clearStencil );

    void setRenderTarget( const zpRenderTargetIdentifier& colorTarget );
    void setRenderTarget( const zpRenderTargetIdentifier& colorTarget, const zpRenderTargetIdentifier& depthTarget );
    void setRenderTargets( const zpRenderTargetIdentifier* colorTargets, zp_size_t colorCount );
    void setRenderTargets( const zpRenderTargetIdentifier* colorTargets, zp_size_t colorCount, const zpRenderTargetIdentifier& depthTarget );
    void setDepthRenderTarget( const zpRenderTargetIdentifier& depthTarget );

    void setViewport( const zpViewport& viewport );
    void setScissorRect( const zpRecti& scissorRect );

    void drawMesh( const zpMatrix4fData& transform, const zpMesh* mesh, const zpMaterial* material, zp_size_t subMeshIndex, zp_int passIndex );

    void blit( const zpRenderTargetIdentifier& src, const zpRenderTargetIdentifier& dest );
    void blit( const zpTexture& src, const zpRenderTargetIdentifier& dest );

    void executeCommandBuffer( const zpRenderCommandBuffer& buffer );

    const zp_byte* getBuffer() const;
    zp_size_t getLength() const;

    void clear();
    void reset();
    void reserve( zp_size_t size );
    void destroy();

private:
    zpDataBuffer m_buffer;
    zp_uint m_uniqueID;
};

#endif // !ZP_RENDER_COMMAND_BUFFER_H
