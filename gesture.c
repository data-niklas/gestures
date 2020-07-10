#include <stdio.h>
#include <stdlib.h>
#include "gesture.h"

unsigned int GESTURE_TOLERANCE = 50;
Gesture *gestures = NULL;
GestureRecorder *gesture_recorder = NULL;

void gesture_start(int x, int y){
    gesture_recorder->x = x;
    gesture_recorder->y = y;
    gesture_recorder->gestures = gesture_copy(gestures);
    gesture_recorder->vectors = NULL;
    gesture_recorder->current = NULL;
}

void gesture_check_paths(){
    GestureVector *vector = gesture_recorder->vectors;
    while(vector != NULL){

        Gesture *gesture = gesture_recorder->gestures;
        while(gesture != NULL){
            GestureVector *gvector = gesture->vector;
            if (gvector != NULL){
                int xdif = gvector->x - vector->x;
                int ydif = gvector->y - vector->y;
                if (xdif < 0)xdif = xdif * -1;
                if (ydif < 0)ydif = ydif * -1;
                if (xdif < GESTURE_TOLERANCE && ydif < GESTURE_TOLERANCE){
                    gesture->vector = gvector->next;
                }
            }
            gesture = gesture->next;
        }

        vector = vector->next;
    }
}

void gesture_stop(){
    gesture_check_paths();
    Gesture *gesture = gesture_recorder->gestures;
    while(gesture != NULL){
        if (gesture->vector == NULL){
            gesture->action(gesture->id);
        }
        gesture = gesture->next;
    }
    gesture_destroy(gesture_recorder->gestures);
    gesture_recorder->gestures = NULL;
}

void gesture_track(int x, int y){
    int xdif = x - gesture_recorder->x;
    int ydif = y - gesture_recorder->y;
    GestureVector *vector = gesture_vector_create();
    vector->x = xdif;
    vector->y = ydif;
    if (gesture_recorder->vectors == NULL){
        gesture_recorder->vectors = vector;
        gesture_recorder->current = vector;
    }
    else{
        gesture_recorder->current->next = vector;
        gesture_recorder->current = vector;
    }
}

int gesture_tracking(){
    return gesture_recorder->vectors != NULL;
}

void gesture_register(Gesture *gesture){
    gesture->next = gestures;
    gestures = gesture;
}

void gesture_unregister(Gesture *gesture){
    gesture_remove(gesture, gestures);
}

void gesture_remove(Gesture *gesture, Gesture *list){
    Gesture *previous = list;
    Gesture *current = NULL;

    if (previous == list)list = previous->next;
    while ((current = previous->next) != NULL){
        if (current == gesture){
            previous->next = gesture->next;
            break;
        }
        previous = current;
    }
    gesture->next = NULL;
}

void gesture_default_action(int id){}

Gesture *gesture_copy(Gesture *gesture){
    if (gesture == NULL)return NULL;
    Gesture *result = gesture_create();
    *result = *gesture;
    gesture = gesture->next;
    result->next = NULL;

    Gesture *current = result;
    Gesture *temp = NULL;
    while (gesture != NULL){
        temp = gesture_create();
        *temp = *gesture;
        temp->next = NULL;
        temp->vector = gesture_vector_copy(temp->vector);
        current->next = temp;
        current = temp;
        gesture = gesture->next;
    }
    return result;
}

GestureVector *gesture_vector_copy(GestureVector *vector){
    if (vector == NULL)return NULL;
    GestureVector *result = gesture_vector_create();
    *result = *vector;
    vector = vector->next;
    result->next = NULL;

    GestureVector *current = result;
    GestureVector *temp = NULL;
    while (vector != NULL){
        temp = gesture_vector_create();
        *temp = *vector;
        temp->next = NULL;
        current->next = temp;
        current = temp;
        vector = vector->next;
    }
    return result;
}

Gesture *gesture_create(){
    Gesture *gesture = malloc(sizeof(Gesture));
    gesture->action = gesture_default_action;
    gesture->vector = NULL;
    gesture->next = NULL;
    gesture->id = 0;
    return gesture;
}

GestureVector *gesture_vector_create(){
    GestureVector *vector = malloc(sizeof(GestureVector));
    vector->x = 0;
    vector->y = 0;
    vector->next = NULL;
    return vector;
}


GestureVector *gesture_vector_create_from_array(int coords[], int points){
    GestureVector *result = NULL;
    GestureVector *temp;
    for (int i = points - 1; i >= 0; i--)
    {
        temp = gesture_vector_create();
        temp->x = coords[i * 2];
        temp->y = coords[i * 2 + 1];
        temp->next = result;
        result = temp;
    }
    
    return result;
}

void gesture_vector_destroy(GestureVector *vector){
    if (vector->next != NULL)gesture_vector_destroy(vector->next);
    free(vector);
}

void gesture_destroy(Gesture *gesture){
    if (gesture->next != NULL)gesture_destroy(gesture->next);
    free(gesture);
}

void gesture_init(){
    gesture_recorder = malloc(sizeof(GestureRecorder));
}

void gesture_finish(){
    if (gesture_recorder->gestures != NULL)gesture_destroy(gesture_recorder->gestures);
    free(gesture_recorder);
}