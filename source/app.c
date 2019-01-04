
typedef struct App {
    f32 delta_t;
} App;

global App *app = 0;
global Platform *platform = 0;

#define RED_OFFSET   2
#define GREEN_OFFSET 1
#define BLUE_OFFSET  0

void
clear_backbuffer() {
    for(u32 j = 0; j < platform->backbuffer_height; ++j) {
        for(u32 i = 0; i < platform->backbuffer_width; ++i) {
            u8 *r = platform->backbuffer + (j*platform->backbuffer_width + i)*3 + RED_OFFSET;
            u8 *g = platform->backbuffer + (j*platform->backbuffer_width + i)*3 + GREEN_OFFSET;
            u8 *b = platform->backbuffer + (j*platform->backbuffer_width + i)*3 + BLUE_OFFSET;
            
            *r -= (u8)((f32)(*r) * 16.f * app->delta_t);
            *g -= (u8)((f32)(*g) * 16.f * app->delta_t);
            *b -= (u8)((f32)(*b) * 16.f * app->delta_t);
        }
    }
}

void
draw_filled_rect(v4 color, v2 position, v2 size) {
    u8 *backbuffer = platform->backbuffer;
    i32 backbuffer_width = (i32)platform->backbuffer_width;
    i32 backbuffer_height = (i32)platform->backbuffer_height;
    
    i32 lower_bound_x = (i32)position.x;
    i32 lower_bound_y = (i32)position.y;
    i32 upper_bound_x = lower_bound_x + (i32)size.x;
    i32 upper_bound_y = lower_bound_y + (i32)size.y;
    i32 pixel_index = 0;
    
    for(i32 j = lower_bound_y;
        j <= upper_bound_y;
        ++j) {
        
        if(j >= 0 && j < backbuffer_height) {
            for(i32 i = lower_bound_x;
                i <= upper_bound_x;
                ++i) {
                
                if(i >= 0 && i < backbuffer_width) {
                    pixel_index = j*backbuffer_width + i;
                    backbuffer[pixel_index*3 + RED_OFFSET]   = (u8)(color.r * 255.f);
                    backbuffer[pixel_index*3 + GREEN_OFFSET] = (u8)(color.g * 255.f);
                    backbuffer[pixel_index*3 + BLUE_OFFSET]  = (u8)(color.b * 255.f);
                }
            }
        }
    }
}

void
draw_filled_circle(v4 color, v2 position, f32 radius) {
    u8 *backbuffer = platform->backbuffer;
    i32 backbuffer_width = (i32)platform->backbuffer_width;
    i32 backbuffer_height = (i32)platform->backbuffer_height;
    
    i32 lower_bound_x = (i32)position.x - (i32)radius;
    i32 lower_bound_y = (i32)position.y - (i32)radius;
    i32 upper_bound_x = (i32)position.x + (i32)radius;
    i32 upper_bound_y = (i32)position.y + (i32)radius;
    i32 pixel_index = 0;
    
    f32 radius_sq = radius*radius;
    
    for(i32 j = lower_bound_y;
        j <= upper_bound_y;
        ++j) {
        
        if(j >= 0 && j < backbuffer_height) {
            for(i32 i = lower_bound_x;
                i <= upper_bound_x;
                ++i) {
                
                if(i >= 0 && i < backbuffer_width) {
                    
                    v2 pixel_position = {
                        (f32)i,
                        (f32)j,
                    };
                    
                    f32 distance_sq =
                        (position.x - pixel_position.x)*(position.x - pixel_position.x) +
                        (position.y - pixel_position.y)*(position.y - pixel_position.y);
                    
                    if(distance_sq <= radius_sq) {
                        pixel_index = j*backbuffer_width + i;
                        backbuffer[pixel_index*3 + RED_OFFSET]   = (u8)(color.r * 255.f);
                        backbuffer[pixel_index*3 + GREEN_OFFSET] = (u8)(color.g * 255.f);
                        backbuffer[pixel_index*3 + BLUE_OFFSET]  = (u8)(color.b * 255.f);
                    }
                }
            }
        }
    }
}

b32
app_update(Platform *platform_) {
    b32 app_should_quit = 0;
    platform = platform_;
    app = platform->permanent_storage;
    {
        app->delta_t = platform->current_time - platform->last_time;
        
        local_persist v2 box_position = {0};
        local_persist v2 box_velocity = {0};
        
        f32 move_speed = 16.f;
        if(platform->key_down[KEY_w]) {
            box_velocity.y += (-move_speed - box_velocity.y) * app->delta_t;
        }
        if(platform->key_down[KEY_a]) {
            box_velocity.x += (-move_speed - box_velocity.x) * app->delta_t;
        }
        if(platform->key_down[KEY_s]) {
            box_velocity.y += (move_speed - box_velocity.y) * app->delta_t;
        }
        if(platform->key_down[KEY_d]) {
            box_velocity.x += (move_speed - box_velocity.x) * app->delta_t;
        }
        
        box_position.x += box_velocity.x;
        box_position.y += box_velocity.y;
        
        clear_backbuffer();
        draw_filled_circle(v4(0.8f, 0.6f, 0, 1), box_position, 32);
        
        local_persist u32 projectile_count = 0;
        local_persist struct {
            v2 position;
            v2 velocity;
            f32 life;
        } projectiles[1024] = {0};
        
        local_persist u32 target_count = 0;
        local_persist struct {
            v2 position;
            f32 sin_pos;
        } targets[1024] = {0};
        
        if(platform->left_mouse_pressed) {
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
        
        if(platform->right_mouse_pressed) {
            targets[target_count++].position = v2(platform->mouse_x, platform->mouse_y);
        }
        
        for(u32 i = 0; i < target_count;) {
            // NOTE(rjf): Make target go in circle
            {
                targets[i].sin_pos += app->delta_t;
                targets[i].position.x += sinf(targets[i].sin_pos);
                targets[i].position.y += cosf(targets[i].sin_pos);
            }
            
            b32 hit = 0;
            
            for(u32 j = 0; j < projectile_count;) {
                if(projectiles[j].position.x >= targets[i].position.x - 32.f &&
                   projectiles[j].position.x <= targets[i].position.x + 32.f &&
                   projectiles[j].position.y >= targets[i].position.y - 32.f &&
                   projectiles[j].position.y <= targets[i].position.y + 32.f) {
                    if(j != projectile_count-1 &&
                       projectile_count > 1) {
                        memory_copy(projectiles+j, projectiles+projectile_count-1,
                                    sizeof(projectiles[0]));
                    }
                    --projectile_count;
                    hit = 1;
                    break;
                }
                else {
                    ++j;
                }
            }
            
            if(hit) {
                if(i != target_count-1 &&
                   target_count > 1) {
                    memory_copy(targets+i, targets+target_count-1,
                                sizeof(targets[0]));
                }
                --target_count;
            }
            else {
                draw_filled_rect(v4(1, 0, 0, 1),
                                 v2(targets[i].position.x - 16, targets[i].position.y - 16),
                                 v2(32, 32));
                ++i;
            }
        }
        
        for(u32 i = 0; i < projectile_count;) {
            projectiles[i].position.x += projectiles[i].velocity.x * app->delta_t;
            projectiles[i].position.y += projectiles[i].velocity.y * app->delta_t;
            projectiles[i].life -= app->delta_t;
            if(projectiles[i].life <= 0.f) {
                if(i != projectile_count-1 &&
                   projectile_count > 1) {
                    memory_copy(projectiles+i, projectiles+projectile_count-1,
                                sizeof(projectiles[0]));
                }
                --projectile_count;
            }
            else {
                draw_filled_circle(v4(1, 1, 1, 1), projectiles[i].position, 4);
                ++i;
            }
        }
    }
    return app_should_quit;
}