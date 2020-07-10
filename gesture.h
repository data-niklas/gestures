#ifndef GESTUREH
#define GESTUREH

#include <stdio.h>
#include <stdlib.h>

typedef struct GestureVector {
    int x;
    int y;
    struct GestureVector *next;
} GestureVector;

typedef struct Gesture {
    int id;
    void (*action)(int id);
    struct GestureVector *vector;
    struct Gesture *next;
} Gesture;

typedef struct GestureRecorder {
    int x;
    int y;
    struct Gesture *gestures;
    struct GestureVector *vectors;
    struct GestureVector *current;
} GestureRecorder;

extern unsigned int GESTURE_TOLERANCE;   
extern Gesture *gestures; 
extern GestureRecorder *gesture_recorder;

void gesture_start(int x, int y);
void gesture_check_paths();
void gesture_stop();
void gesture_track(int x, int y);
int gesture_tracking();

void gesture_register(Gesture *gesture);
void gesture_unregister(Gesture *gesture);
void gesture_remove(Gesture *gesture, Gesture *list);

void gesture_default_action(int id);

Gesture *gesture_copy(Gesture *gesture);
GestureVector *gesture_vector_copy(GestureVector *vector);
Gesture *gesture_create();
GestureVector *gesture_vector_create();
GestureVector *gesture_vector_create_from_array(int coords[], int points);
void gesture_vector_destroy(GestureVector *vector);
void gesture_destroy(Gesture *gesture);
void gesture_init();
void gesture_finish();

#endif