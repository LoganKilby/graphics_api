#define ImGuiUtil_WantCaptureMouse() (ImGui::GetIO().WantCaptureMouse)

void ImGuiUtil_ShowTexture(GL_Texture2D *texture, char *window_name) {
    ImGui::Begin(window_name);
    ImGui::Text("width: %d", texture->width);
    ImGui::Text("height: %d", texture->height);
    ImGui::Image((void *)(intptr_t)texture->id, ImVec2((f32)texture->width, (f32)texture->height));
    ImGui::End();
}


