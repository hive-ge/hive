/**
 * Convenience functions and objects to use when
 * writing vulkan code.
 *
 * Copyright 2020 Anthony C Weathersby
 *
 * Refer to LICENSE.md for licensing information.
 *
 * Code modified from https://vulkan-tutorial.com/
 */
#pragma once

#ifdef HIVE_USE_VULKAN
#define GLFW_INCLUDE_VULKAN

#include "primitive/log.h"
#include <cstring>
#include <functional>
#include <set>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#define HIVE_STRING_NAME "HIVE_GAME_ENGINE"


namespace hive
{

    namespace graphic
    {

    }
    // Main VK instance
    VkInstance vk_instance;
    VkSurfaceKHR vk_surface;
    VkQueue vk_presentation_queue;
    VkQueue vk_graphics_queue;
    VkSwapchainKHR swapChain;
    VkFormat swapchain_image_format;
    VkExtent2D swapchain_extent;

    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;

    static VkPhysicalDevice vk_physical_device = VK_NULL_HANDLE;
    static VkDevice vk_logical_device          = VK_NULL_HANDLE;

    typedef bool(VKenvironmentSpecificSurfaceCreationFunction)(VkInstance, VkSurfaceKHR *);
    typedef void(VKenvironmentGetRequiredExtension)(unsigned * extension_count,
                                                    const char *** extension_names);

    static const std::vector<const char *> default_validation_layers = {
        /*"VK_LAYER_KHRONOS_validation"*/};


    bool initVulkanInstance(
        std::function<VKenvironmentGetRequiredExtension> reqExt,

        const char * app_name = "default_app", unsigned major_version = 0,
        unsigned minor_version = 0, unsigned patch_version = 0,

        const std::vector<const char *> & validation_layer_names = default_validation_layers)
    {


        VkApplicationInfo app_info = {};

        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

        app_info.apiVersion = VK_API_VERSION_1_1;

        app_info.pEngineName = HIVE_STRING_NAME;

        app_info.applicationVersion = VK_MAKE_VERSION(major_version, minor_version, patch_version);

        app_info.pEngineName = app_name;

        app_info.engineVersion =
            VK_MAKE_VERSION(HIVE_MAJOR_VERSION, HIVE_MINOR_VERSION, HIVE_PATCH_VERSION);


        VkInstanceCreateInfo create_info = {};

        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

        create_info.pApplicationInfo = &app_info;


        /**
         * Not a fan of using glfw at this point, but tut gotta tut.
         * TODO - make FW agnostic if possible. Otherwise, us define check for GLFW
         */
        unsigned extension_count;
        const char ** required_extensions;

        reqExt(&extension_count, &required_extensions);

        create_info.enabledExtensionCount   = extension_count;
        create_info.ppEnabledExtensionNames = required_extensions;


        /**
         * --- VALIDATION LAYERS ---
         */

        unsigned layer_count;

        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

        std::vector<VkLayerProperties> available_layers(layer_count);

        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

        for (auto needed_layer : validation_layer_names) {
            for (auto layer : available_layers) {
                __LOG(std::string(layer.layerName));
                if (strcmp(layer.layerName, needed_layer) == 0) goto found;
            }
            __ERROR("Could not get validation layer for:" + std::string(needed_layer));
            return false;
        found:;
        }

        create_info.enabledLayerCount = static_cast<uint32_t>(validation_layer_names.size());

        // create_info.ppEnabledLayerNames = validation_layer_names.data();

        /**
         * -- Initialize Vkvk_instance --
         */

        VkResult vk_instance_creation_result = vkCreateInstance(&create_info, NULL, &vk_instance);

        if (vk_instance_creation_result != VK_SUCCESS) {
            switch (vk_instance_creation_result) {
            case VK_ERROR_OUT_OF_HOST_MEMORY:
                __ERROR("Could not create vk vk_instance: VK_ERROR_OUT_OF_HOST_MEMORY thrown.");
                break;
            case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                __ERROR("Could not create vk vk_instance: VK_ERROR_OUT_OF_DEVICE_MEMORY thrown.");
                break;
            case VK_ERROR_INITIALIZATION_FAILED:
                __ERROR("Could not create vk vk_instance: VK_ERROR_INITIALIZATION_FAILED thrown.");
                break;
            case VK_ERROR_LAYER_NOT_PRESENT:
                __ERROR("Could not create vk vk_instance: VK_ERROR_LAYER_NOT_PRESENT thrown.");
                break;
            case VK_ERROR_EXTENSION_NOT_PRESENT:
                __ERROR("Could not create vk vk_instance: VK_ERROR_EXTENSION_NOT_PRESENT thrown.");
                break;
            case VK_ERROR_INCOMPATIBLE_DRIVER:
                __ERROR("Could not create vk vk_instance: VK_ERROR_INCOMPATIBLE_DRIVER thrown.");
                break;
            }
            return false;
        }

        /**
         * Query Extensions
         */
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

        std::vector<VkExtensionProperties> extensions(extension_count);

        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

        __LOG("VULKAN EXTENSIONS: ");

        for (const auto ext : extensions) __LOG("\t" + std::string(ext.extensionName));

        return true;
    }


    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
    };

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto & queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;

            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vk_surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }

            i++;
        }

        return indices;
    }

    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    bool checkDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                             availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto & extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device)
    {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vk_surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_surface, &formatCount,
                                                 details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk_surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                                      details.presentModes.data());
        }

        bool swapChainAdequate = false;


        return details;
    }

    bool isDeviceSuitable(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices = findQueueFamilies(device);

        bool extensionsSupported = checkDeviceExtensionSupport(device);

        bool swapChainAdequate = false;

        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
            swapChainAdequate =
                !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && swapChainAdequate && extensionsSupported;
    }

    /**
     * -- Initializes VULKAN Device -- Physical & Logical --
     */
    bool initVulkanDevice(
        const std::vector<const char *> & validation_layer_names = default_validation_layers)
    {

        /**
         */


        /**
         * Setup Physical device -- Do checks for support heere
         */

        unsigned device_count = 0;

        vkEnumeratePhysicalDevices(vk_instance, &device_count, nullptr);

        if (device_count < 1) {
            __ERROR("No vulkan devices available");
            return false;
        }

        std::vector<VkPhysicalDevice> devices(device_count);

        vkEnumeratePhysicalDevices(vk_instance, &device_count, devices.data());


        for (auto device : devices) {
            VkPhysicalDeviceProperties device_properties;
            VkPhysicalDeviceFeatures device_features;
            vkGetPhysicalDeviceProperties(device, &device_properties);
            vkGetPhysicalDeviceFeatures(device, &device_features);

            if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                vk_physical_device = device;
                break;
            }
        }

        VkBool32 present_support = false;

        vkGetPhysicalDeviceSurfaceSupportKHR(vk_physical_device, 0, vk_surface, &present_support);

        if (!present_support) return false;

        /**
         * -- QUEUE Families --
         */

        VkDeviceQueueCreateInfo device_queue_info = {};

        uint32_t queue_family_count = 0;

        vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &queue_family_count, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);

        vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &queue_family_count,
                                                 queue_families.data());
        float queue_priority = 1.0;
        unsigned queue_index = 0;
        int i                = 0;
        for (auto queue : queue_families) {

            if (queue.queueFlags & VK_QUEUE_GRAPHICS_BIT) queue_index = i;
            i++;
        }


        device_queue_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        device_queue_info.queueFamilyIndex = queue_index;
        device_queue_info.queueCount       = 1;
        device_queue_info.pQueuePriorities = &queue_priority;


        /**
         * -- Logical Device --
         */

        QueueFamilyIndices indices = findQueueFamilies(vk_physical_device);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(),
                                                  indices.presentFamily.value()};

        float queuePriority = 1.0f;

        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex        = queueFamily;
            queueCreateInfo.queueCount              = 1;
            queueCreateInfo.pQueuePriorities        = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures device_features = {};

        VkDeviceCreateInfo logical_device_create_info      = {};
        logical_device_create_info.pEnabledFeatures        = &device_features;
        logical_device_create_info.queueCreateInfoCount    = queueCreateInfos.size();
        logical_device_create_info.pQueueCreateInfos       = queueCreateInfos.data();
        logical_device_create_info.enabledLayerCount       = validation_layer_names.size();
        logical_device_create_info.ppEnabledLayerNames     = validation_layer_names.data();
        logical_device_create_info.enabledExtensionCount   = deviceExtensions.size();
        logical_device_create_info.ppEnabledExtensionNames = deviceExtensions.data();

        VkResult logical_device_creation_result = vkCreateDevice(
            vk_physical_device, &logical_device_create_info, nullptr, &vk_logical_device);

        if (logical_device_creation_result != VK_SUCCESS) {
            switch (logical_device_creation_result) {
            case VK_ERROR_OUT_OF_HOST_MEMORY:
                __ERROR("Could not create vk vk_instance: VK_ERROR_OUT_OF_HOST_MEMORY thrown.");
                break;
            case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                __ERROR("Could not create vk vk_instance: VK_ERROR_OUT_OF_DEVICE_MEMORY thrown.");
                break;
            case VK_ERROR_INITIALIZATION_FAILED:
                __ERROR("Could not create vk vk_instance: VK_ERROR_INITIALIZATION_FAILED thrown.");
                break;
            case VK_ERROR_EXTENSION_NOT_PRESENT:
                __ERROR("Could not create vk vk_instance: VK_ERROR_EXTENSION_NOT_PRESENT thrown.");
                break;
            case VK_ERROR_FEATURE_NOT_PRESENT:
                __ERROR("Could not create vk vk_instance: VK_ERROR_FEATURE_NOT_PRESENT thrown.");
                break;
            case VK_ERROR_TOO_MANY_OBJECTS:
                __ERROR("Could not create vk vk_instance: VK_ERROR_TOO_MANY_OBJECTS thrown.");
                break;
            case VK_ERROR_DEVICE_LOST:
                __ERROR("Could not create vk vk_instance: VK_ERROR_DEVICE_LOST thrown.");
                break;
            }
            return false;
        }

        vkGetDeviceQueue(vk_logical_device, indices.graphicsFamily.value(), 0,
                         &vk_presentation_queue);
        vkGetDeviceQueue(vk_logical_device, indices.presentFamily.value(), 0, &vk_graphics_queue);

        return true;
    };

    /**
     * -- SWAP CHAIN --
     */

    VkSurfaceFormatKHR
    chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> & availableFormats)
    {
        for (const auto & availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR
    chooseSwapPresentMode(const std::vector<VkPresentModeKHR> & availablePresentModes)
    {
        for (const auto & availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities)
    {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        } else {
            VkExtent2D actualExtent = {720, 480};

            actualExtent.width =
                std::max(capabilities.minImageExtent.width,
                         std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height =
                std::max(capabilities.minImageExtent.height,
                         std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }
    void createSwapChain()
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(vk_physical_device);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode     = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent                = chooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 &&
            imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface                  = vk_surface;

        createInfo.minImageCount    = imageCount;
        createInfo.imageFormat      = surfaceFormat.format;
        createInfo.imageColorSpace  = surfaceFormat.colorSpace;
        createInfo.imageExtent      = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices    = findQueueFamilies(vk_physical_device);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),
                                         indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices   = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform   = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode    = presentMode;
        createInfo.clipped        = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(vk_logical_device, &createInfo, nullptr, &swapChain) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(vk_logical_device, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(vk_logical_device, swapChain, &imageCount, swapChainImages.data());

        swapchain_image_format = surfaceFormat.format;
        swapchain_extent       = extent;
    }


    /**
     * -- Image Views
     */

    void createImageViews()
    {
        swapChainImageViews.resize(swapChainImages.size());

        for (size_t i = 0; i < swapChainImages.size(); i++) {
            VkImageViewCreateInfo createInfo           = {};
            createInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image                           = swapChainImages[i];
            createInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format                          = swapchain_image_format;
            createInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel   = 0;
            createInfo.subresourceRange.levelCount     = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount     = 1;

            if (vkCreateImageView(vk_logical_device, &createInfo, nullptr,
                                  &swapChainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image views!");
            }
        }
    }


    bool initVulkanSurface(std::function<VKenvironmentSpecificSurfaceCreationFunction> tpr)
    {
        if (!(tpr(vk_instance, &vk_surface))) return false;

        return true;
    }


    bool vulkanDestroy()
    {
        if (vk_instance != VK_NULL_HANDLE) {
            if (vk_logical_device != VK_NULL_HANDLE) vkDeviceWaitIdle(vk_logical_device);
            vkDestroyInstance(vk_instance, nullptr);
        }

        return true;
    }

    struct ShaderData {
    };

    struct Pipeline {

      public:
        struct RenderPass {

            VkRenderPass vk_renderpass = VK_NULL_HANDLE;
            bool READY                 = false;

          public:
            RenderPass()
            {
                // Structure specifying an attachment description
                // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkAttachmentDescription.html
                VkAttachmentDescription attachment = {};


                VkSubpassDescription subpass = {};

                /*
                Structure specifying parameters of a newly created render pass
                https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkRenderPassCreateInfo.html
                typedef struct VkRenderPassCreateInfo {
                    VkStructureType                   sType;
                    const void*                       pNext;
                    VkRenderPassCreateFlags           flags;
                    uint32_t                          attachmentCount;
                    const VkAttachmentDescription*    pAttachments;
                    uint32_t                          subpassCount;
                    const VkSubpassDescription*       pSubpasses;
                    uint32_t                          dependencyCount;
                    const VkSubpassDependency*        pDependencies;
                } VkRenderPassCreateInfo;
                */
                VkRenderPassCreateInfo render_pass_info = {};
                render_pass_info.subpassCount           = 1;
                render_pass_info.pSubpasses             = &subpass;

                if (vkCreateRenderPass(vk_logical_device, &render_pass_info, nullptr,
                                       &render_pass) != VK_SUCCESS) {
                    throw std::runtime_error("failed to create render pass!");
                }
            }

            ~RenderPass()
            {
                if (render_pass != VK_NULL_HANDLE)
                    vkDestroyRenderPass(vk_logical_device, render_pass, nullptr);
            }
        } * rp;

        VkPipeline vk_pipeline;

        Pipeline(ShaderData data, RenderPass * rp) : rp(rp)
        {


            // Structure specifying parameters of a newly created graphics pipeline
            // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkGraphicsPipelineCreateInfo.html
            /*
            VkStructureType                                  sType;
            const void*                                      pNext;
            VkPipelineCreateFlags                            flags;
            uint32_t                                         stageCount;
            const VkPipelineShaderStageCreateInfo*           pStages;
            const VkPipelineVertexInputStateCreateInfo*      pVertexInputState;
            const VkPipelineInputAssemblyStateCreateInfo*    pInputAssemblyState;
            const VkPipelineTessellationStateCreateInfo*     pTessellationState;
            const VkPipelineViewportStateCreateInfo*         pViewportState;
            const VkPipelineRasterizationStateCreateInfo*    pRasterizationState;
            const VkPipelineMultisampleStateCreateInfo*      pMultisampleState;
            const VkPipelineDepthStencilStateCreateInfo*     pDepthStencilState;
            const VkPipelineColorBlendStateCreateInfo*       pColorBlendState;
            const VkPipelineDynamicStateCreateInfo*          pDynamicState;
            VkPipelineLayout                                 layout;
            VkRenderPass                                     renderPass;
            uint32_t                                         subpass;
            VkPipeline                                       basePipelineHandle;
            int32_t                                          basePipelineIndex;
            */
            VkPipelineShaderStageCreateInfo pStages                     = {};
            VkPipelineVertexInputStateCreateInfo vertex_input_state     = {};
            VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {};
            VkPipelineTessellationStateCreateInfo tessellation_state    = {};
            VkPipelineViewportStateCreateInfo view_port_state           = {};
            VkPipelineRasterizationStateCreateInfo rasterization_state  = {};
            VkPipelineMultisampleStateCreateInfo multisample_state      = {};
            VkPipelineDepthStencilStateCreateInfo depth_stencil_state   = {};
            VkPipelineColorBlendStateCreateInfo color_blend_state       = {};
            VkPipelineDynamicStateCreateInfo dynamic_state              = {};


            // Structure specifying parameters of a newly created pipeline vertex input state
            // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPipelineVertexInputStateCreateInfo.html
            VkPipelineVertexInputStateCreateInfo vertex_input_state = {};
            vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            vertex_input_state.vertexBindingDescriptionCount   = 0;
            vertex_input_state.pVertexBindingDescriptions      = nullptr; // Optional
            vertex_input_state.vertexAttributeDescriptionCount = 0;
            vertex_input_state.pVertexAttributeDescriptions    = nullptr; // Optional

            VkGraphicsPipelineCreateInfo pipeline_info = {};
            pipeline_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipeline_info.pVertexInputState   = &vertex_input_state;
            pipeline_info.pInputAssemblyState = &input_assembly_state;
            pipeline_info.pViewportState      = &view_port_state;
            pipeline_info.pRasterizationState = &rasterization_state;
            pipeline_info.pMultisampleState   = &multisample_state;
            pipeline_info.pDepthStencilState  = &depth_stencil_state; // Optional
            pipeline_info.pColorBlendState    = &color_blend_state;
            pipeline_info.pDynamicState       = &dynamic_state; // Optional

            pipeline_info.renderPass         = rp->vk_renderpass;
            pipeline_info.subpass            = 0;
            pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
            pipeline_info.basePipelineIndex  = -1;             // Optional

            if (vkCreateGraphicsPipelines(vk_logical_device, VK_NULL_HANDLE, 1, &pipeline_info,
                                          nullptr, &vk_pipeline) != VK_SUCCESS) {
                throw std::runtime_error("failed to create graphics pipeline!");
            }
        }
        /**
         * Adds a shader stage to the pipeline object.
         */
        void addShader(const std::vector<char> & spir_v_data)
        {
            VkShaderModuleCreateInfo createInfo = {};
            createInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            createInfo.codeSize                 = spir_v_data.size();
            createInfo.pCode = reinterpret_cast<const uint32_t *>(spir_v_data.data());

            VkShaderModule shaderModule;
            if (vkCreateShaderModule(vk_logical_device, &createInfo, nullptr, &shaderModule) !=
                VK_SUCCESS) {
                throw std::runtime_error("failed to create shader module!");
            }
        }

        ~Pipeline()
        {
            if (vk_pipeline != VK_NULL_HANDLE)
                vkDestroyPipeline(vk_logical_device, vk_pipeline, nullptr);
        }
    };

    struct ViewPort {
        VkViewport viewport                             = {};
        VkRect2D scissor                                = {};
        VkPipelineViewportStateCreateInfo viewportState = {};
    };


    struct Buffer {
        Buffer() {}
        ~Buffer() {}

        void write(void * data)
        {
            /**
                vkMapMemory(device, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
                    memcpy(data, vertices.data(), (size_t) bufferInfo.size);
                vkUnmapMemory(device, vertexBufferMemory);
            */
        }
    }; // namespace hive


} // namespace hive

#endif