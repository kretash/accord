#include "accord/editor.h"
#include "accord/context.h"

Editor::Editor( SDL_Window* window ) : m_window( window ) {
  ImGui_ImplSdlGL3_Init( window );
}

void Editor::awake() {
  m_context = Context::get_instance();
}

void Editor::start() {

}

void Editor::prepare() {

}

void Editor::update() {

}

void Editor::late_update() {

}

void Editor::render() {

  bool show_fps = true;
  ImGui_ImplSdlGL3_NewFrame( m_window );

  ImGui::BeginMainMenuBar();

  if( ImGui::BeginMenu( "File" ) ) {
    ImGui::EndMenu();
  }
  if( ImGui::BeginMenu( "Edit" ) ) {
    if( ImGui::MenuItem( "Undo", "CTRL+Z" ) ) {}
    if( ImGui::MenuItem( "Redo", "CTRL+Y", false, false ) ) {}  // Disabled item
    ImGui::Separator();
    if( ImGui::MenuItem( "Cut", "CTRL+X" ) ) {}
    if( ImGui::MenuItem( "Copy", "CTRL+C" ) ) {}
    if( ImGui::MenuItem( "Paste", "CTRL+V" ) ) {}
    ImGui::EndMenu();
  }

  if( ImGui::BeginMenu( "Windows" ) ) {
    m_w_hierarchy = ImGui::MenuItem( "Hierarchy" );
    m_w_inspector = ImGui::MenuItem( "Inspector" );

    ImGui::EndMenu();
  }
  ImGui::EndMainMenuBar();



  if( show_fps ) {
    ImGui::Begin( "Voxels", &show_fps );

    int32_t fps = -1;

    if( m_context != nullptr ) {
      fps = ( int32_t ) m_context->m_cumulative_fps;
    }

    ImGui::Text( std::string( "FPS:" + std::to_string( fps ) ).c_str() );

    ImGui::End();
  }

  ImGui::Render();
}

void Editor::shutdown() {
  ImGui_ImplSdlGL3_Shutdown();
}

Editor::~Editor() {

}