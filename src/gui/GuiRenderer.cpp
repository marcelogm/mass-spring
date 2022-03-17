            #include "gui.hpp"

void GuiRenderer::render() {
	auto const config = Configuration::getInstance();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %.2f", Frametime::getInstance()->getFramerate());
	ImGui::End();

	ImGui::Begin("Light", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat3("Position", value_ptr(config->getLight()->lightPosition), -20, 20, "%.0f", ImGuiSliderFlags_None);
	ImGui::SliderFloat("Ambient Strength", &config->getLight()->ambientLightStrength, 0, 1, "%.2f", ImGuiSliderFlags_None);
	ImGui::SliderFloat("Difffuse Strength", &config->getLight()->diffuseLightStrength, 0, 1, "%.2f", ImGuiSliderFlags_None);
	ImGui::End();

	ImGui::Begin("Simulation", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat("Gravity Modifier", &config->getSimulationParams()->gravityModifier, 0.00001f, 100.0f, "%.5f", ImGuiSliderFlags_Logarithmic);
	ImGui::SliderFloat3("Wind Direction", value_ptr(config->getSimulationParams()->windDirection), -1, 1, "%.0f", ImGuiSliderFlags_None);
	ImGui::SliderFloat("Wind Modifier", &config->getSimulationParams()->windModifier, 0.00001f, 100.0f, "%.5f", ImGuiSliderFlags_Logarithmic);
	ImGui::SliderFloat("Time step", &config->getSimulationParams()->step, 0.001f, 0.1f, "%.5f", ImGuiSelectableFlags_None);
	ImGui::SliderFloat("Stiffness", &config->getSimulationParams()->stiffness, 0.000001, 1.0, "%.8f", ImGuiSelectableFlags_None);
	
	ImGui::SliderFloat("Sphere Modifier", &config->getDebug()->radiusModifier, 0.001f, 5.0f, "%.3f", ImGuiSelectableFlags_None);
	ImGui::SliderFloat("Collission Stiffness", &config->getDebug()->collisionStiffness, 0.000001, 1.00000, "%.8f", ImGuiSelectableFlags_None);

	ImGui::Text("Broad Phase: %d", config->getDebug()->broadPhase);
	ImGui::Text("Narrow Phase: %d", config->getDebug()->narrowPhase);

	ImGui::SliderFloat("Sphere position X", &config->getDebug()->debugPosition.x, -2, 8, "%.8f", ImGuiSliderFlags_None);
	ImGui::SliderFloat("Sphere position Y", &config->getDebug()->debugPosition.y, -2, 8, "%.8f", ImGuiSliderFlags_None);
	ImGui::SliderFloat("Sphere position Z", &config->getDebug()->debugPosition.z, -2, 8, "%.8f", ImGuiSliderFlags_None);
	
	ImGui::Checkbox("Collision", &config->getSimulationParams()->collision);
	ImGui::Checkbox("Debug Collision", &config->getDebug()->showCollision);

	ImGui::End();

	ImGui::Begin("Render mode", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::RadioButton("Triangulos", (int *)&config->getOpenGLConfiguration()->mode, 0);
	ImGui::RadioButton("Lines", (int*)&config->getOpenGLConfiguration()->mode, 1);
	ImGui::RadioButton("Points", (int*)&config->getOpenGLConfiguration()->mode, 2);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GuiRenderer::GuiRenderer(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
	ImGui::StyleColorsDark();
}
