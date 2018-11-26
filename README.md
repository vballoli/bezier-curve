# Bezier Curve

Bezier curve is a curve of degree n with n+1 control points. This project aims to implement a bezier curve using de casteljau's algorithm in OpenGL.

## Algorithm

Lerp is recursively applied to the control points until it converges to a single point.

Addition of point: Add's x,y to the 2D control array.

Deletion of point: Removes x,y from the 2D control array within a TOUCH_BUFFER range.

Dragging a point: Stores previous touch and current point on lift off within the TOUCH_BUFFER and changes the point.

t is controlled using RES parameter in the code.

## Run instructions

`gcc bezier.c -lLG -lGLU -lglut`

### Keyboard shortcuts:

`a: adds point on the cuurve
 d: delete's point on the curve
 b: draggable points on the curve`

### Mouse shortcuts

`Left click only`
