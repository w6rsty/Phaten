#pragma once

union SDL_Event;

namespace Pt {

void ImGuiInit();
void ImGuiShutdown();
void ImGuiProcessEvent(const SDL_Event& event);

void ImGuiBegin();
void ImGuiEnd();

} // namespace Pt