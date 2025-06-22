#include <Game.h>
#include <Renderer/RVR.h>

void RVR::CreateInstance()
{
	XrApplicationInfo appInfo;
	const auto applicationName = pGame->GetApplicationName();
	strncpy_s(appInfo.applicationName, applicationName.size(), applicationName.c_str(), XR_MAX_APPLICATION_NAME_SIZE);
	appInfo.applicationVersion = 1;
	std::string engineName = "Limbic Engine";
	strncpy_s(appInfo.engineName, engineName.size(), engineName.c_str(), XR_MAX_ENGINE_NAME_SIZE);
	appInfo.apiVersion = XR_CURRENT_API_VERSION;

	instanceExtensions.emplace_back(XR_EXT_DEBUG_UTILS_EXTENSION_NAME);
	instanceExtensions.emplace_back(XR_KHR_VULKAN_ENABLE_EXTENSION_NAME);

	// Get all the API Layers from the OpenXR runtime.
	uint32_t apiLayerCount = 0;
	std::vector<XrApiLayerProperties> apiLayerProperties;
	OPENXR_CHECK(xrEnumerateApiLayerProperties(0, &apiLayerCount, nullptr), "Failed to enumerate ApiLayerProperties.");
	apiLayerProperties.resize(apiLayerCount, {XR_TYPE_API_LAYER_PROPERTIES});
	OPENXR_CHECK(xrEnumerateApiLayerProperties(apiLayerCount, &apiLayerCount, apiLayerProperties.data()),
		"Failed to enumerate ApiLayerProperties.");

	// Check the requested API layers against the ones from the OpenXR. If found add it to the Active API Layers.
	for (auto& requestLayer : apiLayers)
	{
		for (const auto& layerProperty : apiLayerProperties)
		{
			if (strcmp(requestLayer.c_str(), layerProperty.layerName) == 0)
			{
				activeAPILayers.push_back(requestLayer.c_str());
				break;
			}
		}
	}

	// Get all the Instance Extensions from the OpenXR instance.
	uint32_t extensionCount = 0;
	std::vector<XrExtensionProperties> extensionProperties;
	OPENXR_CHECK(xrEnumerateInstanceExtensionProperties(nullptr, 0, &extensionCount, nullptr),
		"Failed to enumerate InstanceExtensionProperties.");
	extensionProperties.resize(extensionCount, {XR_TYPE_EXTENSION_PROPERTIES});
	OPENXR_CHECK(xrEnumerateInstanceExtensionProperties(nullptr, extensionCount, &extensionCount, extensionProperties.data()),
		"Failed to enumerate InstanceExtensionProperties.");

	// Check the requested Instance Extensions against the ones from the OpenXR runtime.
	// If an extension is found add it to Active Instance Extensions.
	// Log error if the Instance Extension is not found.
	for (auto& requestedInstanceExtension : instanceExtensions)
	{
		bool found = false;
		for (const auto& extensionProperty : extensionProperties)
		{
			// strcmp returns 0 if the strings match.
			if (strcmp(requestedInstanceExtension.c_str(), extensionProperty.extensionName) == 0)
			{
				activeInstanceExtensions.push_back(requestedInstanceExtension.c_str());
				found = true;
				break;
			}
		}
		if (!found)
		{
			throw std::runtime_error("[ERROR] Failed to find OpenXR instance extension: " + requestedInstanceExtension);
		}
	}
}

void RVR::DestroyInstance()
{
}

void RVR::CreateDebugMessenger()
{
}

void RVR::DestroyDebugMessenger()
{
}

void RVR::GetInstanceProperties()
{
}

void RVR::GetSystemID()
{
}

void RVR::PollSystemEvents()
{
}
