#include "Game.h"

/**** Static debug callback configuration. ****/

static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

static void DestroyDebugUtilsMessengerEXT(
	VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		std::cout << "[VERBOSE] " << pCallbackData->pMessage << std::endl;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		std::cout << "[INFO] " << pCallbackData->pMessage << std::endl;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		std::cout << "[WARNING] " << pCallbackData->pMessage << std::endl;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		std::cout << "[ERROR] " << pCallbackData->pMessage << std::endl;
		break;
	default:
		std::cout << "[DEFAULT] " << pCallbackData->pMessage << std::endl;
		break;
	}
	return VK_FALSE;
}

/**** Public Interface ****/

Game::Game(std::string applicationName, IOSystem* pIO)
{
	this->applicationName = applicationName;
	this->pIO = pIO;
}

void Game::OnInit()
{
	CreateVkInstance();

	renderSystem = std::make_unique<RenderSystem>();
	resourceSystem = std::make_unique<ResourceSystem>(renderSystem.get());
	
	uint32 w, h;
	ioSystem->GetFramebufferSize(w, h);
	renderSystem->Init(applicationName.c_str(), w, h, ioSystem->GetWindow(), ioSystem->GetProcess());

	worldSystem = std::make_unique<WorldSystem>();
	worldSystem->hIO = ioSystem;
	worldSystem->hResource = resourceSystem.get();
	worldSystem->hRender = renderSystem.get();
	worldSystem->LoadFromJSON("C:/Users/alekm/Desktop/Outpost731/Map/Test.json");

	EEntity** entities;
	uint32 entityCount;
	worldSystem->GetEntities(entities, entityCount);
	for (uint32 i = 0; i < entityCount; i++)
	{
		entities[i]->OnInit();
	}
}

void Game::Run()
{
	OnInit();

	auto lastTickTime = std::chrono::high_resolution_clock::now();

	while (true)
	{
		auto currentTickTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float, std::chrono::seconds::period>(currentTickTime - lastTickTime).count();
		lastTickTime = currentTickTime;

		pIO->OnTick(dt);

		EEntity** entities;
		uint32 entityCount = 0;
		worldSystem->GetEntities(entities, entityCount);
		for (uint32 i = 0; i < entityCount; i++)
		{
			entities[i]->OnTick(dt);
		}

		renderSystem->OnDrawStart();

		for (uint32 i = 0; i < entityCount; i++)
		{
			entities[i]->OnDraw();
		}

		renderSystem->OnDrawEnd();
	}
}

void Game::CreateVkInstance()
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = applicationName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Limbic Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	std::vector<const char*> extensions = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME};

	if (bEnableValidationLayers)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	if (!CheckVkInstanceExtensionSupport(extensions))
		std::cout << "[ERROR] Required vulkan instance extensions not available!\n";

	createInfo.enabledExtensionCount = static_cast<uint32>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (bEnableValidationLayers && !CheckValidationLayerSupport(validationLayers))
		std::cout << "[ERROR] Vulkan validation layers requested, but not available!\n";

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (bEnableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
		ConfigureDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &vkInstance) != VK_SUCCESS)
	{
		std::cout << "[ERROR] Failed to create Vulkan instance!\n";
	}
}

bool Game::CheckVkInstanceExtensionSupport(const std::vector<const char*>& extensions)
{
	uint32 availableExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

	for (const char* extensionName : extensions)
	{
		bool bExtensionFound = false;

		for (const auto& extensionProperties : availableExtensions)
		{
			if (strcmp(extensionName, extensionProperties.extensionName) == 0)
			{
				bExtensionFound = true;
				break;
			}
		}
		if (!bExtensionFound)
			return false;
	}
	return true;
}