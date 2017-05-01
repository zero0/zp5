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

void zpDebugGUI::setup( zpInput* input )
{
    m_input = input;

    m_style.backgroundColor = zpColor32::Grey75;
    m_style.foregroundColor = zpColor32::Grey25;
    m_style.contentColor = zpColor32::White;
    m_style.fontSize = 12;
}

void zpDebugGUI::teardown()
{
    m_input = ZP_NULL;
}

void zpDebugGUI::startGUI()
{
    zp_size_t index = m_widgets.size();

    m_widgetStack.pushBack( index );
    zpDebugGUIWidget& window = m_widgets.pushBackEmpty();

    window.frameUpdated = npos;
    window.parentWidget = npos;

    window.layout.rect = { 0, 0, m_screenSize.x, m_screenSize.y };
    window.layout.offset = { 0, 0 };
    window.layout.padding = { 0, 0 };
    window.layout.margin = { 0, 0 };
    window.layout.expandWidth = false;
    window.layout.expandHeight = false;

    window.type = ZP_WIDGET_TYPE_CONTAINER;
    window.style = m_style;
}

void zpDebugGUI::update( zp_float dt, zp_float rt )
{
    ++m_frame;
}

void zpDebugGUI::render( zpRenderingContext* ctx )
{

}

void zpDebugGUI::endGUI()
{
    m_widgetStack.popBack();
    ZP_ASSERT( m_widgetStack.isEmpty(), "" );

    m_widgets.clear();
    m_widgetStack.clear();
}

void zpDebugGUI::beginWindow( zpRecti& area )
{
    ZP_ASSERT( m_widgetStack.size() == 1, "" );

    zp_size_t parent = m_widgetStack.back();
    zp_size_t index = m_widgets.size();

    m_widgetStack.pushBack( index );
    zpDebugGUIWidget& window = m_widgets.pushBackEmpty();
    
    window.frameUpdated = npos;
    window.parentWidget = parent;
    
    window.layout.rect = area;
    window.layout.offset = { 0, 0 };
    window.layout.padding = { 0, 0 };
    window.layout.margin = { 2, 2 };
    window.layout.expandWidth = false;
    window.layout.expandHeight = false;

    window.type = ZP_WIDGET_TYPE_WINDOW;
    window.style = m_style;

    m_widgets[ parent ].children.pushBack( index );
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

void zpDebugGUI::label( const zp_char* label )
{
    ZP_ASSERT( !m_widgetStack.isEmpty(), "" );

    zp_size_t parent = m_widgetStack.back();
    zp_size_t index = m_widgets.size();

    m_widgetStack.pushBack( index );
    zpDebugGUIWidget& window = m_widgets.pushBackEmpty();

    window.frameUpdated = npos;
    window.parentWidget = parent;

    window.layout.rect = { 0, 0, 0, 0 };
    window.layout.offset = { 0, 0 };
    window.layout.padding = { 0, 0 };
    window.layout.margin = { 2, 2 };
    window.layout.expandWidth = true;
    window.layout.expandHeight = true;

    window.type = ZP_WIDGET_TYPE_LABEL;
    window.style = m_style;

    window.text = label;

    m_widgets[ parent ].children.pushBack( index );
}

zp_bool zpDebugGUI::button( const zp_char* label )
{
    ZP_ASSERT( !m_widgetStack.isEmpty(), "" );

    zp_size_t parent = m_widgetStack.back();
    zp_size_t index = m_widgets.size();

    m_widgetStack.pushBack( index );
    zpDebugGUIWidget& window = m_widgets.pushBackEmpty();

    window.frameUpdated = npos;
    window.parentWidget = parent;

    window.layout.rect = { 0, 0, 0, 0 };
    window.layout.offset = { 0, 0 };
    window.layout.padding = { 0, 0 };
    window.layout.margin = { 2, 2 };
    window.layout.expandWidth = true;
    window.layout.expandHeight = true;

    window.type = ZP_WIDGET_TYPE_BUTTON;
    window.style = m_style;

    window.text = label;
    m_widgets[ parent ].children.pushBack( index );

    return false;
}

void zpDebugGUI::box()
{

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
