#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include <imgui.h>
#include <backends/imgui_impl_opengl2.h>
#include <backends/imgui_impl_sdl2.h>

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void Engine::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	// Init IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();


	int w, h;
	SDL_GetWindowSize(m_window, &w, &h);
	m_WindowSize = { float(w), float(h) };
}

void Engine::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	ImGuiIO& io = ImGui::GetIO();

	SceneManager::GetInstance().Render();
	// Render scene's onimgui, after the scene has been rendered, so the scene can be rendered on top of the imgui
	SceneManager::GetInstance().OnImGui();  

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		//SDL_GLContext ctx = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();

		//SDL_GL_MakeCurrent(m_window, ctx);
	}

	SDL_RenderPresent(m_renderer);
}

void Engine::Renderer::Destroy()
{
	// ImGui
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	// Renderer
	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}

	
}

void Engine::Renderer::RenderTexture(const Texture2D& texture, float x, float y) const
{
	const glm::ivec2 textureSize{ texture.GetSize() };
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(-y + m_WindowSize.y - textureSize.y);
	dst.w = textureSize.x;
	dst.h = textureSize.y;

	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Engine::Renderer::RenderTexture(const Texture2D& texture, float x, float y, float angle, const glm::vec2& center, SDL_RendererFlip flip) const
{
	const glm::ivec2 textureSize{ texture.GetSize() };
	RenderTexture(texture, glm::vec4{}, glm::vec4(x, y, textureSize.x, textureSize.y), angle, center, flip);
}

void Engine::Renderer::RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(-y + m_WindowSize.y - height);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Engine::Renderer::RenderTexture(const Texture2D& texture, const glm::vec4& srcRect, const glm::vec4 dstRect, float angle, const glm::vec2& center, SDL_RendererFlip flip) const
{
	// Z = width, W = height in the vec4

	const glm::ivec2 textureSize{ texture.GetSize() };

	SDL_Rect src{};
	src.x = static_cast<int>(srcRect.x);
	src.y = static_cast<int>(textureSize.y - srcRect.y);  // Offset with texture size, because i inverted sdl to work with Y up
	src.w = static_cast<int>(srcRect.z);
	src.h = static_cast<int>(srcRect.w);

	// Replace src with full texture size if 0
	if (srcRect.z <= FLT_EPSILON || srcRect.w <= FLT_EPSILON)
	{
		src.w = textureSize.x;
		src.h = textureSize.y;
	}
	src.y -= src.h;  // Correction for src height

	SDL_Point sdlCenter{};
	sdlCenter.x = static_cast<int>(center.x * dstRect.z);
	sdlCenter.y = static_cast<int>((dstRect.w) - (center.y * dstRect.w));

	SDL_Rect dst{};
	dst.x = static_cast<int>(dstRect.x - sdlCenter.x);
	dst.y = static_cast<int>(-dstRect.y + m_WindowSize.y - sdlCenter.y);
	dst.w = static_cast<int>(dstRect.z);
	dst.h = static_cast<int>(dstRect.w);

	// Replace dst with full texture size if 0
	if (dstRect.z <= FLT_EPSILON || dstRect.w <= FLT_EPSILON)
	{
		dst.w = textureSize.x;
		dst.h = textureSize.y;
	}

	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst, angle, &sdlCenter, flip);

}

inline SDL_Renderer* Engine::Renderer::GetSDLRenderer() const { return m_renderer; }

glm::vec2 Engine::Renderer::GetWindowSize() const
{
	return m_WindowSize;
}
