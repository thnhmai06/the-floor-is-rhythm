#include <iostream>  
#include <vector>  
#include <SDL3/SDL.h>  
#include <quill/LogMacros.h>  
#include <expections/sdl.h>  
#include "logging/init.h"  

constexpr char NAME[] = "The Floor is Rhythm";  
constexpr bool DEBUG_MODE = true;  

int32_t main(int32_t argc, char* argv[])  
{  
   LogSystem::init();  
   try  
   {  
       if (!SDL_Init(SDL_INIT_VIDEO)) throw SDL_Exception::Critical::SDL_InitSDL_Failed();

       SDL_Window* window{ SDL_CreateWindow(NAME, 1366, 768, SDL_WINDOW_HIDDEN) };  
       if (!window) throw SDL_Exception::Critical::SDL_CreateWindow_Failed();  

       SDL_GPUDevice* device{ SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_MSL | SDL_GPU_SHADERFORMAT_DXIL, DEBUG_MODE, nullptr) };  
       if (!device) throw SDL_Exception::Critical::SDL_CreateGPUDevice_Failed();  

       if (!SDL_ClaimWindowForGPUDevice(device, window)) throw SDL_Exception::Critical::SDL_ClaimWindowForGPUDevice_Failed();  

       std::cout << SDL_GetGPUDeviceDriver(device);  

       SDL_ShowWindow(window);  

       bool is_running = true;  
       SDL_Event event;  
       // main loop  
       while (is_running)  
       {  
           // event loop  
           while (SDL_PollEvent(&event))  
           {  
               switch (event.type)  
               {  
               case SDL_EVENT_QUIT:  
                   is_running = false;  
                   break;  
               default: break;  
               }  

               // work here  
               SDL_GPUCommandBuffer* command_buffer{ SDL_AcquireGPUCommandBuffer(device) };  
               if (!command_buffer) throw SDL_Exception::Critical::SDL_AcquireGPUCommandBuffer_Failed();  

               SDL_GPUTexture* swapchains_texture;  
               SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, window, &swapchains_texture, nullptr, nullptr);  
               if (swapchains_texture)  
               {  
                   SDL_GPUColorTargetInfo color_target{};  
                   color_target.texture = swapchains_texture;  
                   color_target.store_op = SDL_GPU_STOREOP_STORE;  
                   color_target.load_op = SDL_GPU_LOADOP_CLEAR;  
                   color_target.clear_color = { 1.0f, 0.0f, 0.0f, 1.0f };  

                   std::vector color_targets{ color_target };  
                   SDL_GPURenderPass* render_pass  
                   {  
                       SDL_BeginGPURenderPass(command_buffer, color_targets.data(), color_targets.size(), nullptr)  
                   };  
                   SDL_EndGPURenderPass(render_pass);  
               }  

               if (!SDL_SubmitGPUCommandBuffer(command_buffer)) throw SDL_Exception::Critical::SDL_SubmitGPUCommandBuffer_Failed();  
           }  
       }  

       return EXIT_SUCCESS;  
   }  
   catch (const std::exception& e)  
   {
       LOG_CRITICAL(LogSystem::logger, "{}", e.what());  
       return EXIT_FAILURE;
   }  
}