#include "zpEngine.h"

zpDebugGUI::zpDebugGUI()
    : m_input( ZP_NULL )
    , m_flags( 0 )
    , m_frame( 0 )
    , m_screenSize( { 960, 640 } )
    , m_style()
    , m_debugFont()
    , m_debugMaterial()
    , m_widgets( 128 )
    , m_widgetStack( 32 )
{
}

zpDebugGUI::~zpDebugGUI()
{
}

void zpDebugGUI::setup( zpInput* input, const zpFontHandle& debugFont )
{
    m_input = input;
    m_debugFont = debugFont;

    m_style.backgroundColor = zpColor32::Grey75;
    m_style.foregroundColor = zpColor32::Grey25;
    m_style.contentColor = zpColor32::White;
    m_style.fontSize = 12;

    m_layout.rect = { 0, 0, m_screenSize.x, m_screenSize.y };
    m_layout.offset = { 0, 0 };
    m_layout.padding = { 2, 2 };
    m_layout.margin = { 2, 2 };
    m_layout.cursor = { 0, 0 };
    m_layout.direction = { 0, 1 };
    m_layout.expandWidth = false;
    m_layout.expandHeight = false;
}

void zpDebugGUI::teardown()
{
    m_input = ZP_NULL;
    m_debugFont.release();

    m_widgets.clear();
    m_widgetStack.clear();
}

void zpDebugGUI::startGUI()
{
    m_widgets.clear();
    m_widgetStack.clear();

    zp_size_t index = m_widgets.size();

    m_widgetStack.pushBack( index );
    zpDebugGUIWidget& window = m_widgets.pushBackEmpty();

    window.frameUpdated = npos;
    window.frameRendered = npos;
    window.parentWidget = npos;

    window.layout = m_layout;
    window.layout.rect = { 0, 0, m_screenSize.x, m_screenSize.y };
    window.layout.cursor = { 0, 0 };
    window.layout.direction = { 0, 1 };

    window.type = ZP_WIDGET_TYPE_CONTAINER;
    window.style = m_style;
}

void zpDebugGUI::update( zp_float dt, zp_float rt )
{
    ++m_frame;
}

void zpDebugGUI::render( zpRenderingContext* ctx )
{
    zp_bool isRenderingText = false;

    ctx->beginDrawImmediate( 0, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR );

    zpRecti orthoRect = { 0, 0, m_screenSize.x, m_screenSize.y };
    zp_float l = static_cast<zp_float>( orthoRect.x );
    zp_float r = static_cast<zp_float>( orthoRect.x + orthoRect.width );
    zp_float t = static_cast<zp_float>( orthoRect.y );
    zp_float b = static_cast<zp_float>( orthoRect.y + orthoRect.height );
    zp_float n = static_cast<zp_float>( -10 );
    zp_float f = static_cast<zp_float>( 10 );

    zpMatrix4f ortho = zpMath::OrthoLH( l, r, t, b, n, f );
    ctx->setTransform( ortho );

    renderWidget( ctx, 0, isRenderingText );

    ctx->endDraw();
}

void zpDebugGUI::endGUI()
{
    m_widgetStack.popBack();
    ZP_ASSERT( m_widgetStack.isEmpty(), "Widgets still on stack" );
}

void zpDebugGUI::beginWindow( zpRecti& area )
{
    ZP_ASSERT( m_widgetStack.size() == 1, "Windows must be at the top level" );

    zp_size_t parent = m_widgetStack.back();
    zp_size_t index = m_widgets.size();

    m_widgetStack.pushBack( index );
    zpDebugGUIWidget& window = m_widgets.pushBackEmpty();
    zpDebugGUIWidget& parentWidget = m_widgets[ parent ];

    window.frameUpdated = npos;
    window.frameRendered = npos;
    window.parentWidget = parent;
    
    window.style = m_style;
    window.layout = m_layout;
    window.layout.rect = area;
    window.layout.cursor = area.position;

    window.type = ZP_WIDGET_TYPE_WINDOW;

    parentWidget.children.pushBack( index );
}

void zpDebugGUI::endWindow()
{
    m_widgetStack.popBack();
}

void zpDebugGUI::beginVertical()
{
    ZP_ASSERT( !m_widgetStack.isEmpty(), "" );

}

void zpDebugGUI::endVertical()
{
    m_widgetStack.popBack();
}

void zpDebugGUI::beginHorizontal()
{

}

void zpDebugGUI::endHorizontal()
{

}

void zpDebugGUI::beginScrollArea( zpVector2i& scroll )
{

}

void zpDebugGUI::endScrollArea()
{

}

void zpDebugGUI::label( const zp_char* text )
{
    ZP_ASSERT( !m_widgetStack.isEmpty(), "" );

    zp_size_t parent = m_widgetStack.back();
    zp_size_t index = m_widgets.size();

    zpDebugGUIWidget& label = m_widgets.pushBackEmpty();
    zpDebugGUIWidget& parentWidget = m_widgets[ parent ];

    label.frameUpdated = npos;
    label.frameRendered = npos;
    label.parentWidget = parent;

    label.style = m_style;
    label.layout = parentWidget.layout;
    label.layout.rect.position = parentWidget.layout.cursor;
    label.layout.rect.width = parentWidget.layout.rect.width;
    label.layout.rect.height = label.style.fontSize + label.layout.margin.y * 2;

    parentWidget.layout.cursor.x += parentWidget.layout.direction.x * ( label.layout.rect.width + parentWidget.layout.margin.x );
    parentWidget.layout.cursor.y += parentWidget.layout.direction.y * ( label.layout.rect.height + parentWidget.layout.margin.y );

    label.type = ZP_WIDGET_TYPE_LABEL;
    zp_size_t len = zp_strlen( text );
    zp_memcpy( label.text, len, text, len );
    label.text[ len ] = '\0';

    parentWidget.children.pushBack( index );
}

zp_bool zpDebugGUI::button( const zp_char* label )
{
    ZP_ASSERT( !m_widgetStack.isEmpty(), "" );

    zp_size_t parent = m_widgetStack.back();
    zp_size_t index = m_widgets.size();

    m_widgetStack.pushBack( index );
    zpDebugGUIWidget& button = m_widgets.pushBackEmpty();
    zpDebugGUIWidget& parentWidget = m_widgets[ parent ];

    button.frameUpdated = npos;
    button.parentWidget = parent;

    button.layout.rect = { 0, 0, 0, 0 };

    button.type = ZP_WIDGET_TYPE_BUTTON;
    button.style = m_style;

    zp_strcpy( button.text, zp_strlen( label ), label );
    parentWidget.children.pushBack( index );

    return false;
}

void zpDebugGUI::box( zp_int width, zp_int height )
{
    ZP_ASSERT( !m_widgetStack.isEmpty(), "" );

    zp_size_t parent = m_widgetStack.back();
    zp_size_t index = m_widgets.size();

    zpDebugGUIWidget& box = m_widgets.pushBackEmpty();
    zpDebugGUIWidget& parentWidget = m_widgets[ parent ];

    box.frameUpdated = npos;
    box.frameRendered = npos;
    box.parentWidget = parent;
    box.type = ZP_WIDGET_TYPE_BOX;

    box.style = m_style;
    box.layout = parentWidget.layout;
    box.layout.rect.position = parentWidget.layout.cursor;
    box.layout.rect.width = width < 0 ? box.layout.rect.width : width;
    box.layout.rect.height = height < 0 ? box.layout.rect.height : height;

    parentWidget.layout.cursor.x += parentWidget.layout.direction.x * ( box.layout.rect.width + parentWidget.layout.margin.x );
    parentWidget.layout.cursor.y += parentWidget.layout.direction.y * ( box.layout.rect.height + parentWidget.layout.margin.y );

    box.text[ 0 ] = '\0';
    parentWidget.children.pushBack( index );
}

void zpDebugGUI::updateWidget( zp_size_t widgetIndex )
{
    zpDebugGUIWidget& widget = m_widgets[ widgetIndex ];
    if( widget.frameUpdated != m_frame )
    {
        widget.frameUpdated = m_frame;

        if( widget.parentWidget != npos )
        {
            zpDebugGUIWidget& parent = m_widgets[ widget.parentWidget ];

        }
    }

    const zp_size_t* childBegin = widget.children.begin();
    const zp_size_t* childEnd = widget.children.end();
    for( ; childBegin != childEnd; ++childBegin )
    {
        updateWidget( *childBegin );
    }
}

void zpDebugGUI::renderWidget( zpRenderingContext* ctx, zp_size_t widgetIndex, zp_bool& isRenderingText )
{
    zpDebugGUIWidget& widget = m_widgets[ widgetIndex ];
    if( widget.frameRendered != m_frame )
    {
        widget.frameRendered = m_frame;

        switch( widget.type )
        {
            case ZP_WIDGET_TYPE_BOX:
            {
                if( isRenderingText )
                {
                    isRenderingText = false;

                    ctx->endDraw();
                    ctx->beginDrawImmediate( 0, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR );

                    zpRecti orthoRect = { 0, 0, m_screenSize.x, m_screenSize.y };
                    zp_float l = static_cast<zp_float>( orthoRect.x );
                    zp_float r = static_cast<zp_float>( orthoRect.x + orthoRect.width );
                    zp_float t = static_cast<zp_float>( orthoRect.y );
                    zp_float b = static_cast<zp_float>( orthoRect.y + orthoRect.height );
                    zp_float n = static_cast<zp_float>( -10 );
                    zp_float f = static_cast<zp_float>( 10 );

                    zpMatrix4f ortho = zpMath::OrthoLH( l, r, t, b, n, f );
                    ctx->setTransform( ortho );
                }

                zpRecti& rect = widget.layout.rect;
                zpVector4fData v0 = { static_cast<zp_float>(rect.x             ), static_cast<zp_float>(rect.y              ), 0.f, 1.f };
                zpVector4fData v1 = { static_cast<zp_float>(rect.x             ), static_cast<zp_float>(rect.y + rect.height), 0.f, 1.f };
                zpVector4fData v2 = { static_cast<zp_float>(rect.x + rect.width), static_cast<zp_float>(rect.y + rect.height), 0.f, 1.f };
                zpVector4fData v3 = { static_cast<zp_float>(rect.x + rect.width), static_cast<zp_float>(rect.y              ), 0.f, 1.f };
                
                zp_ushort vertexCount = static_cast<zp_ushort>( ctx->getVertexCount() );

                ctx->addVertexData( v0, m_style.backgroundColor );
                ctx->addVertexData( v1, m_style.backgroundColor );
                ctx->addVertexData( v2, m_style.backgroundColor );
                ctx->addVertexData( v3, m_style.backgroundColor );

                v0 = { static_cast<zp_float>( rect.x              + 2 ), static_cast<zp_float>( rect.y               + 2 ), 0.f, 1.f };
                v1 = { static_cast<zp_float>( rect.x              + 2 ), static_cast<zp_float>( rect.y + rect.height - 2 ), 0.f, 1.f };
                v2 = { static_cast<zp_float>( rect.x + rect.width - 2 ), static_cast<zp_float>( rect.y + rect.height - 2 ), 0.f, 1.f };
                v3 = { static_cast<zp_float>( rect.x + rect.width - 2 ), static_cast<zp_float>( rect.y               + 2 ), 0.f, 1.f };
                
                ctx->addVertexData( v0, m_style.foregroundColor );
                ctx->addVertexData( v1, m_style.foregroundColor );
                ctx->addVertexData( v2, m_style.foregroundColor );
                ctx->addVertexData( v3, m_style.foregroundColor );

                ctx->addQuadIndex( vertexCount + 0, vertexCount + 1, vertexCount + 2, vertexCount + 3 );
                ctx->addQuadIndex( vertexCount + 4, vertexCount + 5, vertexCount + 6, vertexCount + 7 );

            } break;

            case ZP_WIDGET_TYPE_LABEL:
            {
                if( !isRenderingText )
                {
                    isRenderingText = true;

                    ctx->endDraw();
                    ctx->beginDrawText( 0, m_debugFont );

                    zpRecti orthoRect = { 0, 0, m_screenSize.x, m_screenSize.y };
                    zp_float l = static_cast<zp_float>( orthoRect.x );
                    zp_float r = static_cast<zp_float>( orthoRect.x + orthoRect.width );
                    zp_float t = static_cast<zp_float>( orthoRect.y );
                    zp_float b = static_cast<zp_float>( orthoRect.y + orthoRect.height );
                    zp_float n = static_cast<zp_float>( -10 );
                    zp_float f = static_cast<zp_float>( 10 );

                    zpMatrix4f ortho = zpMath::OrthoLH( l, r, t, b, n, f );
                    ctx->setTransform( ortho );
                }

                zpVector4fData pos = { static_cast<zp_float>(widget.layout.rect.x), static_cast<zp_float>(widget.layout.rect.y), 0.f, 1.f };
                ctx->addTextShadow( pos, widget.text, m_style.fontSize, 2, m_style.contentColor, m_style.contentColor, { 1, 1, 0, 0 }, zpColor32::Black );
            } break;

            case ZP_WIDGET_TYPE_BUTTON:
            {

            } break;
        }
    }

    const zp_size_t* childBegin = widget.children.begin();
    const zp_size_t* childEnd = widget.children.end();
    for( ; childBegin != childEnd; ++childBegin )
    {
        renderWidget( ctx, *childBegin, isRenderingText );
    }
}
