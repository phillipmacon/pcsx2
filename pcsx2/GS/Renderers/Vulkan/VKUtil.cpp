/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2023  PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PrecompiledHeader.h"

#include "GS/Renderers/Vulkan/VKUtil.h"
#include "GS/Renderers/Vulkan/VKContext.h"

#include "common/Assertions.h"
#include "common/Console.h"
#include "common/StringUtil.h"

#include <cmath>

void Vulkan::SafeDestroyFramebuffer(VkFramebuffer& fb)
{
	if (fb != VK_NULL_HANDLE)
	{
		vkDestroyFramebuffer(g_vulkan_context->GetDevice(), fb, nullptr);
		fb = VK_NULL_HANDLE;
	}
}

void Vulkan::SafeDestroyShaderModule(VkShaderModule& sm)
{
	if (sm != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(g_vulkan_context->GetDevice(), sm, nullptr);
		sm = VK_NULL_HANDLE;
	}
}

void Vulkan::SafeDestroyPipeline(VkPipeline& p)
{
	if (p != VK_NULL_HANDLE)
	{
		vkDestroyPipeline(g_vulkan_context->GetDevice(), p, nullptr);
		p = VK_NULL_HANDLE;
	}
}

void Vulkan::SafeDestroyPipelineLayout(VkPipelineLayout& pl)
{
	if (pl != VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(g_vulkan_context->GetDevice(), pl, nullptr);
		pl = VK_NULL_HANDLE;
	}
}

void Vulkan::SafeDestroyDescriptorSetLayout(VkDescriptorSetLayout& dsl)
{
	if (dsl != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorSetLayout(g_vulkan_context->GetDevice(), dsl, nullptr);
		dsl = VK_NULL_HANDLE;
	}
}

void Vulkan::SafeDestroyBufferView(VkBufferView& bv)
{
	if (bv != VK_NULL_HANDLE)
	{
		vkDestroyBufferView(g_vulkan_context->GetDevice(), bv, nullptr);
		bv = VK_NULL_HANDLE;
	}
}

void Vulkan::SafeDestroyImageView(VkImageView& iv)
{
	if (iv != VK_NULL_HANDLE)
	{
		vkDestroyImageView(g_vulkan_context->GetDevice(), iv, nullptr);
		iv = VK_NULL_HANDLE;
	}
}

void Vulkan::SafeDestroySampler(VkSampler& samp)
{
	if (samp != VK_NULL_HANDLE)
	{
		vkDestroySampler(g_vulkan_context->GetDevice(), samp, nullptr);
		samp = VK_NULL_HANDLE;
	}
}

void Vulkan::SafeDestroySemaphore(VkSemaphore& sem)
{
	if (sem != VK_NULL_HANDLE)
	{
		vkDestroySemaphore(g_vulkan_context->GetDevice(), sem, nullptr);
		sem = VK_NULL_HANDLE;
	}
}

void Vulkan::SafeFreeGlobalDescriptorSet(VkDescriptorSet& ds)
{
	if (ds != VK_NULL_HANDLE)
	{
		g_vulkan_context->FreeGlobalDescriptorSet(ds);
		ds = VK_NULL_HANDLE;
	}
}

void Vulkan::BufferMemoryBarrier(VkCommandBuffer command_buffer, VkBuffer buffer, VkAccessFlags src_access_mask,
	VkAccessFlags dst_access_mask, VkDeviceSize offset, VkDeviceSize size, VkPipelineStageFlags src_stage_mask,
	VkPipelineStageFlags dst_stage_mask)
{
	VkBufferMemoryBarrier buffer_info = {
		VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER, // VkStructureType    sType
		nullptr, // const void*        pNext
		src_access_mask, // VkAccessFlags      srcAccessMask
		dst_access_mask, // VkAccessFlags      dstAccessMask
		VK_QUEUE_FAMILY_IGNORED, // uint32_t           srcQueueFamilyIndex
		VK_QUEUE_FAMILY_IGNORED, // uint32_t           dstQueueFamilyIndex
		buffer, // VkBuffer           buffer
		offset, // VkDeviceSize       offset
		size // VkDeviceSize       size
	};

	vkCmdPipelineBarrier(command_buffer, src_stage_mask, dst_stage_mask, 0, 0, nullptr, 1, &buffer_info, 0, nullptr);
}

void Vulkan::AddPointerToChain(void* head, const void* ptr)
{
	VkBaseInStructure* last_st = static_cast<VkBaseInStructure*>(head);
	while (last_st->pNext)
	{
		if (last_st->pNext == ptr)
			return;

		last_st = const_cast<VkBaseInStructure*>(last_st->pNext);
	}

	last_st->pNext = static_cast<const VkBaseInStructure*>(ptr);
}

const char* Vulkan::VkResultToString(VkResult res)
{
	switch (res)
	{
		case VK_SUCCESS:
			return "VK_SUCCESS";

		case VK_NOT_READY:
			return "VK_NOT_READY";

		case VK_TIMEOUT:
			return "VK_TIMEOUT";

		case VK_EVENT_SET:
			return "VK_EVENT_SET";

		case VK_EVENT_RESET:
			return "VK_EVENT_RESET";

		case VK_INCOMPLETE:
			return "VK_INCOMPLETE";

		case VK_ERROR_OUT_OF_HOST_MEMORY:
			return "VK_ERROR_OUT_OF_HOST_MEMORY";

		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			return "VK_ERROR_OUT_OF_DEVICE_MEMORY";

		case VK_ERROR_INITIALIZATION_FAILED:
			return "VK_ERROR_INITIALIZATION_FAILED";

		case VK_ERROR_DEVICE_LOST:
			return "VK_ERROR_DEVICE_LOST";

		case VK_ERROR_MEMORY_MAP_FAILED:
			return "VK_ERROR_MEMORY_MAP_FAILED";

		case VK_ERROR_LAYER_NOT_PRESENT:
			return "VK_ERROR_LAYER_NOT_PRESENT";

		case VK_ERROR_EXTENSION_NOT_PRESENT:
			return "VK_ERROR_EXTENSION_NOT_PRESENT";

		case VK_ERROR_FEATURE_NOT_PRESENT:
			return "VK_ERROR_FEATURE_NOT_PRESENT";

		case VK_ERROR_INCOMPATIBLE_DRIVER:
			return "VK_ERROR_INCOMPATIBLE_DRIVER";

		case VK_ERROR_TOO_MANY_OBJECTS:
			return "VK_ERROR_TOO_MANY_OBJECTS";

		case VK_ERROR_FORMAT_NOT_SUPPORTED:
			return "VK_ERROR_FORMAT_NOT_SUPPORTED";

		case VK_ERROR_SURFACE_LOST_KHR:
			return "VK_ERROR_SURFACE_LOST_KHR";

		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";

		case VK_SUBOPTIMAL_KHR:
			return "VK_SUBOPTIMAL_KHR";

		case VK_ERROR_OUT_OF_DATE_KHR:
			return "VK_ERROR_OUT_OF_DATE_KHR";

		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";

		case VK_ERROR_VALIDATION_FAILED_EXT:
			return "VK_ERROR_VALIDATION_FAILED_EXT";

		case VK_ERROR_INVALID_SHADER_NV:
			return "VK_ERROR_INVALID_SHADER_NV";

		default:
			return "UNKNOWN_VK_RESULT";
	}
}

void Vulkan::LogVulkanResult(const char* func_name, VkResult res, const char* msg, ...)
{
	std::va_list ap;
	va_start(ap, msg);
	std::string real_msg = StringUtil::StdStringFromFormatV(msg, ap);
	va_end(ap);

	Console.Error("(%s) %s (%d: %s)", func_name, real_msg.c_str(), static_cast<int>(res), VkResultToString(res));
}
