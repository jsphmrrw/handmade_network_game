
enum {
#define key(name, str) KEY_##name,
#include "platform_key_list.inc"
    KEY_MAX
};

typedef struct Platform {
    b32 quit;
    
    // NOTE(rjf): Storage
    struct {
        void *permanent_storage;
        u32 permanent_storage_size;
    };
    
    // NOTE(rjf): Backbuffer
    struct {
        u8 *backbuffer;
        u32 backbuffer_width;
        u32 backbuffer_height;
    };
    
    // NOTE(rjf): Time
    struct {
        f32 frames_per_second_target;
        f32 current_time;
        f32 last_time;
    };
    
    // NOTE(rjf): Input
    struct {
        b32 key_down[KEY_MAX];
        f32 mouse_x;
        f32 mouse_y;
        b32 left_mouse_down;
        b32 left_mouse_pressed;
        b32 right_mouse_down;
        b32 right_mouse_pressed;
    };
    
} Platform;