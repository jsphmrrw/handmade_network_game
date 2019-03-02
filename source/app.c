#include "ui.h"

typedef struct App
{
    UI ui;
    f32 delta_t;
}
App;

global App *app = 0;
global Platform *platform = 0;

#include "debug.c"
#include "render.c"
#include "ui.c"

#define UI_SRC_ID 1000

internal b32
AppUpdate(Platform *platform_)
{
    b32 app_should_quit = 0;
    platform = platform_;
    app = platform->permanent_storage;
    app->delta_t = platform->current_time - platform->last_time;
    
    ClearBackbuffer();
    
    UIInput ui_input = {0};
    {
        ui_input.cursor_x = platform->mouse_x;
        ui_input.cursor_y = platform->mouse_y;
        ui_input.left_cursor_down = platform->left_mouse_down;
        ui_input.right_cursor_down = platform->right_mouse_down;
    }
    
    UIBeginFrame(&app->ui, &ui_input);
    
    // NOTE(rjf): // NOTE(rjf): // NOTE(rjf): 
    // NOTE(rjf): TESTING UI STUFF
    {
        UIPushColumn(&app->ui, v2(32, 32), v2(256, 48));
        {
            if(UIButton(&app->ui, UIIDGen(), "uhasfdfiuhsa"))
            {
                
            }
            local_persist f32 slider_value = 0;
            slider_value = UISlider(&app->ui, UIIDGen(), "sdiojfsdjif", slider_value);
            
            DrawFilledCircle(v4(slider_value, slider_value, slider_value, 1.f),
                             v2(512.f, 512.f),
                             64.f);
        }
        UIPopColumn(&app->ui);
    }
    // NOTE(rjf): // NOTE(rjf): // NOTE(rjf): 
    // NOTE(rjf): // NOTE(rjf): // NOTE(rjf): 
    
    UIEndFrame(&app->ui);
    
#if 0
    {
        local_persist v2 box_position = {0};
        local_persist v2 box_velocity = {0};
        
        f32 move_speed = 16.f;
        if(platform->key_down[KEY_w])
        {
            box_velocity.y += (-move_speed - box_velocity.y) * app->delta_t;
        }
        if(platform->key_down[KEY_a])
        {
            box_velocity.x += (-move_speed - box_velocity.x) * app->delta_t;
        }
        if(platform->key_down[KEY_s])
        {
            box_velocity.y += (move_speed - box_velocity.y) * app->delta_t;
        }
        if(platform->key_down[KEY_d])
        {
            box_velocity.x += (move_speed - box_velocity.x) * app->delta_t;
        }
        
        box_position.x += box_velocity.x;
        box_position.y += box_velocity.y;
        
        DrawFilledCircle(v4(0.8f, 0.6f, 0, 1), box_position, 32);
        
        local_persist u32 projectile_count = 0;
        local_persist struct
        {
            v2 position;
            v2 velocity;
            f32 life;
        }
        projectiles[1024] = {0};
        
        local_persist u32 target_count = 0;
        local_persist struct
        {
            v2 position;
            f32 sin_pos;
        }
        targets[1024] = {0};
        
        if(platform->left_mouse_pressed)
        {
            f32 projectile_speed = 512.f;
            
            v2 velocity = {
                platform->mouse_x - box_position.x,
                platform->mouse_y - box_position.y,
            };
            
            f32 distance_between_mouse_and_box_position = sqrtf(velocity.x*velocity.x + velocity.y*velocity.y);
            
            velocity.x /= distance_between_mouse_and_box_position;
            velocity.y /= distance_between_mouse_and_box_position;
            
            velocity.x *= projectile_speed;
            velocity.y *= projectile_speed;
            
            projectiles[projectile_count].position = box_position;
            projectiles[projectile_count].velocity = velocity;
            projectiles[projectile_count].life = 1.f;
            ++projectile_count;
        }
        
        if(platform->right_mouse_pressed)
        {
            targets[target_count++].position = v2(platform->mouse_x, platform->mouse_y);
        }
        
        for(u32 i = 0; i < target_count;)
        {
            // NOTE(rjf): Make target go in circle
            {
                targets[i].sin_pos += app->delta_t;
                targets[i].position.x += sinf(targets[i].sin_pos);
                targets[i].position.y += cosf(targets[i].sin_pos);
            }
            
            b32 hit = 0;
            
            for(u32 j = 0; j < projectile_count;)
            {
                if(projectiles[j].position.x >= targets[i].position.x - 32.f &&
                   projectiles[j].position.x <= targets[i].position.x + 32.f &&
                   projectiles[j].position.y >= targets[i].position.y - 32.f &&
                   projectiles[j].position.y <= targets[i].position.y + 32.f)
                {
                    if(j != projectile_count-1 && projectile_count > 1)
                    {
                        MemoryCopy(projectiles+j, projectiles+projectile_count-1,
                                   sizeof(projectiles[0]));
                    }
                    --projectile_count;
                    hit = 1;
                    break;
                }
                else
                {
                    ++j;
                }
            }
            
            if(hit)
            {
                if(i != target_count-1 && target_count > 1)
                {
                    MemoryCopy(targets+i, targets+target_count-1,
                               sizeof(targets[0]));
                }
                --target_count;
            }
            else
            {
                DrawFilledRect(v4(1, 0, 0, 1),
                               v2(targets[i].position.x - 16, targets[i].position.y - 16),
                               v2(32, 32));
                ++i;
            }
        }
        
        for(u32 i = 0; i < projectile_count;)
        {
            projectiles[i].position.x += projectiles[i].velocity.x * app->delta_t;
            projectiles[i].position.y += projectiles[i].velocity.y * app->delta_t;
            projectiles[i].life -= app->delta_t;
            if(projectiles[i].life <= 0.f)
            {
                if(i != projectile_count-1 && projectile_count > 1)
                {
                    MemoryCopy(projectiles+i, projectiles+projectile_count-1,
                               sizeof(projectiles[0]));
                }
                --projectile_count;
            }
            else
            {
                DrawFilledCircle(v4(1, 1, 1, 1), projectiles[i].position, 4);
                ++i;
            }
        }
    }
#endif
    
    return app_should_quit;
}

#undef UI_SRC_ID