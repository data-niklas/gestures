# Gestures - A simple gesture recognition C library
## Use Case
- Can be used in combination with xcb / xlib or similar libraries to detect mouse gestures
- Can be used with any programm, which provides coordinates

## How to use
- Initialize the library
- Register gestures

- At some point start the gesture tracking
- Feed the library coordinates
- Stop the gesture tracking and run all actions of the recognized gestures

- Free the gestures
```
gesture_init();

Gesture *right = gesture_create();
right->id = 0;
right->action = some_function;

GestureVector *vector = gesture_vector_create();
vector->x = 200;
vector->y = 0;

right->vector = vector;

gesture_register(right);


Gesture *complex = gesture_create();
complex->id = 1;
complex->action = some_function;

int points[] = {50, -100, 100, 0, 0, 0};
GestureVector *vector = gesture_vector_create_from_array(points, 3);
complex->vector = vector;

gesture_register(complex);

gesture_start(startx, starty);

//feed it coordinates several times
gesture_track(x, y);
//
gesture_stop();


// On programm finish
gesture_unregister(right);
gesture_unregister(complex);
gesture_destroy(right);
gesture_destroy(complex);
gesture_finish();
```

- The tolerance can be changed
- e.g.: `GESTURE_TOLERANCE = 100;`
