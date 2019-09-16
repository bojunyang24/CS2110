/**
* @file graphics.c
* @author YOUR NAME HERE
* @date DATE HERE
* @brief A graphics library for drawing geometry, for Homework 9 of Georgia Tech
*        CS 2110, Fall 2018.
*/

// Please take a look at the header file below to understand what's required for
// each of the functions.
#include "graphics.h"

// Don't touch this. It's used for sorting for the drawFilledPolygon function.
int int_cmp(const void *a, const void *b)
{
    const int *ia = (const int *)a;
    const int *ib = (const int *)b;
    return *ia  - *ib;
}

Pixel noFilter(Pixel c) {
    // Don't touch this.
    return c;
}

// TODO: Complete according to the prototype in graphics.h
Pixel greyscaleFilter(Pixel c) {
    Pixel red = c & 0x001F;
    Pixel green = (c & 0x03E0) >> 5;
    Pixel blue = (c & 0x7C00) >> 10;
    c = ((red * 77) + (green * 151) + (blue * 28)) >> 8;
    green = c << 5;
    blue = c << 10;
    c = c | green;
    c = c | blue;
    return c;
}

// TODO: Complete according to the prototype in graphics.h
Pixel redOnlyFilter(Pixel c) {
    return c & 0x001F;
}

// TODO: Complete according to the prototype in graphics.h
Pixel brighterFilter(Pixel c) {
    Pixel red = c & 0x001F;
    Pixel green = (c & 0x03E0) >> 5;
    Pixel blue = (c & 0x7C00) >> 10;
    Pixel max = 0x001F;
    Pixel redD = max - red;
    red = red + (redD >> 1);
    Pixel greenD = max - green;
    green = green + (greenD >> 1);
    Pixel blueD = max - blue;
    blue = blue + (blueD >> 1);
    c = red;
    c = c | (green << 5);
    c = c | (blue << 10);
    return c;
}


// TODO: Complete according to the prototype in graphics.h
void drawPixel(Screen *screen, Vector coordinates, Pixel pixel) {
    int width = screen -> size.x;
    int height = screen -> size.y;
    int x = coordinates.x; // the column
    if (x < 0 || x >= width) {
        return;
    }
    int y = coordinates.y; // the row
    if (y < 0 || y >= height) {
        return;
    }
    Pixel* buffer = screen -> buffer;
    int index = (y * width) + x;
    buffer[index] = pixel;
    // UNUSED(screen);
    // UNUSED(coordinates);
    // UNUSED(pixel);
}

// TODO: Complete according to the prototype in graphics.h
void drawFilledRectangle(Screen *screen, Rectangle *rectangle) {
    int top_leftX = rectangle -> top_left.x;
    int top_leftY = rectangle -> top_left.y;
    int width = rectangle -> size.x;
    int height = rectangle -> size.y;
    Pixel color = rectangle -> color;
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            Vector target = {top_leftX + c, top_leftY + r};
            drawPixel(screen, target, color);
        }
    }
    // UNUSED(screen);
    // UNUSED(rectangle);
}

// TODO: Complete according to the prototype in graphics.h
void drawLine(Screen *screen, Line *line) {
    int x1 = line -> start.x;
    int x = x1;
    int y1 = line -> start.y;
    int y = y1;
    int x2 = line -> end.x;
    int y2 = line -> end.y;
    Pixel color = line -> color;
    // if start and end same just plot 1 point
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int signx;
    int signy;
    if ((x2 - x1) == 0) {
        signx = 0;
    } else if ((x2 - x1) < 0) {
        signx = -1;
    } else {
        signx = 1;
    }
    if ((y2 - y1) == 0) {
        signy = 0;
    } else if ((y2 - y1) < 0) {
        signy = -1;
    } else {
        signy = 1;
    }



    int changed = -1;
    if (dy > dx) {
        int temp = dx;
        dx = dy;
        dy = temp;
        changed = 1;
    }
    int e = (2 * dy) - dx;
    for (int i = 1; i <= dx; i++) {
        Vector target = {x, y};
        drawPixel(screen, target, color);
        while (e >= 0) {
            if (changed == 1) {
                x = x + signx;
            } else {
                y = y + signy;
            }
            e = e - 2 * dx;
        }
        if (changed == 1) {
            y += signy;
        } else {
            x += signx;
        }
        e = e + 2 * dy;
    }
    Vector target = {x2, y2};
    drawPixel(screen, target, color);
    // UNUSED(screen);
    // UNUSED(line);
}

// TODO: Complete according to the prototype in graphics.h
void drawPolygon(Screen *screen, Polygon *polygon) {
    Vector* vertices = polygon -> vertices;
    int n = polygon -> num_vertices; // n is number of vertices of polygon
    Pixel color = polygon -> color;
    for (int i = 0; i < n; i++) {
        if (i != n - 1) {
            Line side = {vertices[i], vertices[i + 1], color};
            drawLine(screen, &side);
        } else {
            Line side = {vertices[i], vertices[0], color};
            drawLine(screen, &side);
        }
    }
    // UNUSED(screen);
    // UNUSED(polygon);
}

// TODO: Complete according to the prototype in graphics.h
void drawFilledPolygon(Screen *screen, Polygon *polygon) {
    UNUSED(screen);
    int min_y = INT_MAX;
    int max_y = INT_MIN;

    // -------------------------------------------------------------------------
    // TODO: Here, write some code that will find the minimum and maximum
    // Y values that belong to vertices of the polygon, and store them as
    // min_y and max_y.
    // -------------------------------------------------------------------------
    Vector* vertices = polygon -> vertices;
    int n = polygon -> num_vertices; // n is number of vertices of polygon
    Pixel color = polygon -> color;
    for (int i = 0; i < n; i++) {
        if (vertices[i].y < min_y) {
            min_y = vertices[i].y;
        }
        if (vertices[i].y > max_y) {
            max_y = vertices[i].y;
        }
    }

    // -------------------------------------------------------------------------
    // END OF TODO
    // -------------------------------------------------------------------------

    // Now we iterate through the rows of our polygon
    for (int row = min_y; row <= max_y; row++) {
        // This variable contains the number of nodes found. We start with 0.
        int nNodes = 0;

        // This array will contain the X coords of the nodes we find.
        // Note that there are at most num_vertices of those. We allocate
        // that much room, but at any time only the first nNodes ints will
        // be our actual data.
        int nodeX[polygon->num_vertices];

        // This loop finds the nodes on this row. It intersects the line
        // segments between consecutive pairs of vertices with the horizontal
        // line corresponding to the row we're on. Don't worry about the
        // details, it just works.
        int j = polygon->num_vertices - 1;
        for (int i = 0; i < polygon->num_vertices; i++) {
            if ((polygon->vertices[i].y < row && polygon->vertices[j].y >= row) ||
                (polygon->vertices[j].y < row && polygon->vertices[i].y >= row)) {
                nodeX[nNodes++] = (polygon->vertices[i].x +
                    (row - polygon->vertices[i].y) *
                    (polygon->vertices[j].x - polygon->vertices[i].x) /
                    (polygon->vertices[j].y - polygon->vertices[i].y));
            }
            j = i;
        }

        // ---------------------------------------------------------------------
        // TODO: Make a call to qsort here to sort the nodeX array we made,
        // from small to large x coordinate. Note that there are nNodes elements
        // that we want to sort, and each is an integer. We give you int_cmp
        // at the top of the file to use as the comparator for the qsort call,
        // so you can just pass it to qsort and not need to write your own
        // comparator.
        // ---------------------------------------------------------------------
        qsort(nodeX, nNodes, sizeof(int), int_cmp);

        // ---------------------------------------------------------------------
        // END OF TODO
        // ---------------------------------------------------------------------



        // ---------------------------------------------------------------------
        // TODO: Now we fill the x coordinates corresponding to the interior of
        // the polygon. Note that after every node we switch sides on the
        // polygon, that is, if we are on the outside, when we pass a node we
        // end up on the inside, and if are inside, we end up on the outside.
        // As a result, all you need to do is start at the 0th node, iterate
        // through all of the even-indexed nodes, and fill until the next node.
        // For example, you need to fill between nodes 0-1, between nodes 2-3,
        // nodes 4-5 etc. but not between nodes 1-2, or nodes 3-4.
        // ---------------------------------------------------------------------
        for (int i = 0; i < nNodes; i = i + 2) {
            for (int j = nodeX[i]; j < nodeX[i + 1]; j++) {
                Vector target = {j, row};
                drawPixel(screen, target, color);
            }
            // Vector start = {nodeX[i], row};
            // Vector end = {nodeX[i + 1], row};
            // Line fillLine = {start, end, color};
            // drawLine(screen, &fillLine);
        }
        // UNUSED(nodeX);

        // ---------------------------------------------------------------------
        // END OF TODO
        // ---------------------------------------------------------------------
    }
}

// TODO: Complete according to the prototype in graphics.h
void drawRectangle(Screen *screen, Rectangle *rectangle) {
    int top_leftX = rectangle -> top_left.x;
    int top_leftY = rectangle -> top_left.y;
    int width = rectangle -> size.x;
    width--;
    int height = rectangle -> size.y;
    height--;
    Pixel color = rectangle -> color;
    Vector vertices[4];
    Vector vertex = {top_leftX, top_leftY};
    vertices[0] = vertex;
    Vector vertex1 = {top_leftX + width, top_leftY};
    vertices[1] = vertex1;
    Vector vertex2 = {top_leftX + width, top_leftY + height};
    vertices[2] = vertex2;
    Vector vertex3 = {top_leftX, top_leftY + height};
    vertices[3] = vertex3;
    Polygon polygon = {vertices, 4, color};
    drawPolygon(screen, &polygon);
    UNUSED(screen);
    UNUSED(rectangle);
}

// TODO: Complete according to the prototype in graphics.h
void drawCircle(Screen *screen, Circle *circle) {
    int mx = circle -> center.x;
    int my = circle -> center.y;
    int radius = circle -> radius;
    Pixel color = circle -> color;
    int x = 0;
    int y = radius;
    int d = 1 - radius;
    while (x <= y) {
        Vector target = {mx + x, my + y};
        drawPixel(screen, target, color);
        target = (Vector) {mx + x, my - y};
        drawPixel(screen, target, color);

        target = (Vector) {mx - x, my + y};
        drawPixel(screen, target, color);
        target = (Vector) {mx - x, my - y};
        drawPixel(screen, target, color);

        target = (Vector) {mx + y, my + x};
        drawPixel(screen, target, color);
        target = (Vector) {mx + y, my - x};
        drawPixel(screen, target, color);

        target = (Vector) {mx - y, my + x};
        drawPixel(screen, target, color);
        target = (Vector) {mx - y, my - x};
        drawPixel(screen, target, color);

        if (d < 0) {
            d = d + 2 * x + 3;
            x = x + 1;
        } else {
            d = d + 2 * (x - y) + 5;
            x = x + 1;
            y = y - 1;
        }
    }
    // UNUSED(screen);
    // UNUSED(circle);
}

// TODO: Complete according to the prototype in graphics.h
void drawFilledCircle(Screen *screen, Circle *circle) {
    int mx = circle -> center.x;
    int my = circle -> center.y;
    int radius = circle -> radius;
    Pixel color = circle -> color;
    int x = 0;
    int y = radius;
    int d = 1 - radius;
    while (x <= y) {
        Vector p1 = {mx + x, my + y};
        Vector p2 = {mx + x, (my > y) ? (my - y) : 0};
        Line line12 = {p1, p2, color};
        drawLine(screen, &line12);

        if (mx >= x) {
            Vector p3 = {mx - x, my + y};
            Vector p4 = {mx - x, (my > y) ? (my - y) : 0};
            Line line34 = {p3, p4, color};
            drawLine(screen, &line34);
        }

        Vector p5 = {mx + y, my + x};
        Vector p6 = {mx + y, (my > x) ? (my - x) : 0};
        Line line56 = {p5, p6, color};
        drawLine(screen, &line56);

        if (mx >= y) {
            Vector p7 = {mx - y, my + x};
            Vector p8 = {mx - y, (my > x) ? (my - x) : 0};
            Line line78 = {p7, p8, color};
            drawLine(screen, &line78);
        }

        if (d < 0) {
            d = d + 2 * x + 3;
            x++;
        } else {
            d = d + 2 * (x - y) + 5;
            x = x + 1;
            y = y - 1;
        }
    }
    // UNUSED(screen);
    // UNUSED(circle);
}

// TODO: Complete according to the prototype in graphics.h
void drawImage(Screen *screen, Image *image, Pixel (*colorFilter)(Pixel)) {
    int x = image -> top_left.x;
    int y = image -> top_left.y;
    int width = image -> size.x;
    int height = image -> size.y;
    Pixel* imageBuffer = image -> buffer;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Vector target = {x + i, y + j};
            Pixel color = colorFilter(imageBuffer[((j) * width) + (i)]);
            drawPixel(screen, target, color);
        }
    }
    // UNUSED(screen);
    // UNUSED(image);
    // UNUSED(colorFilter);
}

// TODO: Complete according to the prototype in graphics.
Image rotateImage(Image *image, int degrees) {
    int rotationFactor = (degrees / 90) % 4;
    if (degrees < 0) {
        rotationFactor += 4;
    }
    Pixel* rotatedBuffer = (Pixel*) malloc(sizeof(Pixel) * (image -> size.x) * (image -> size.y));
    Pixel* originalBuffer = image -> buffer;
    int originalWidth = image -> size.x;
    int originalHeight = image -> size.y;
    Image rotatedImage;
    if (rotationFactor == 1) {
        rotatedImage.size.x = image -> size.y;
        rotatedImage.size.y = image -> size.x;
        rotatedImage.top_left = image -> top_left;
        for (int r = 0; r < originalHeight; r++) {
            for (int c = 0; c < originalWidth; c++) {
                int originalIndex = r * originalWidth + c;
                int rotatedIndex = (originalWidth - c - 1) * rotatedImage.size.x + r;
                rotatedBuffer[rotatedIndex] = originalBuffer[originalIndex];
            }
        }
        rotatedImage.buffer = rotatedBuffer;
        return rotatedImage;
    } else if (rotationFactor == 2) {
        rotatedImage.size.x = image -> size.x;
        rotatedImage.size.y = image -> size.y;
        rotatedImage.top_left = image -> top_left;
        for (int r = 0; r < originalHeight; r++) {
            for (int c = 0; c < originalWidth; c++) {
                int originalIndex = r * originalWidth + c;
                int rotatedIndex = (originalHeight - r - 1) * rotatedImage.size.x + (originalWidth - c - 1);
                rotatedBuffer[rotatedIndex] = originalBuffer[originalIndex];
            }
        }
        rotatedImage.buffer = rotatedBuffer;
        return rotatedImage;
    } else if (rotationFactor == 3) {
        rotatedImage.size.x = image -> size.y;
        rotatedImage.size.y = image -> size.x;
        rotatedImage.top_left = image -> top_left;
        for (int r = 0; r < originalHeight; r++) {
            for (int c = 0; c < originalWidth; c++) {
                int originalIndex = r * originalWidth + c;
                int rotatedIndex = c * rotatedImage.size.x + (originalHeight - r - 1);
                rotatedBuffer[rotatedIndex] = originalBuffer[originalIndex];
            }
        }
        rotatedImage.buffer = rotatedBuffer;
        return rotatedImage;
    } else {
        rotatedImage.size.x = image -> size.x;
        rotatedImage.size.y = image -> size.y;
        rotatedImage.top_left = image -> top_left;
        for (int r = 0; r < originalHeight; r++) {
            for (int c = 0; c < originalWidth; c++) {
                int originalIndex = r * originalWidth + c;
                int rotatedIndex = originalIndex;
                rotatedBuffer[rotatedIndex] = originalBuffer[originalIndex];
            }
        }
        rotatedImage.buffer = rotatedBuffer;
        return rotatedImage;
    }
    UNUSED(image);
    UNUSED(degrees);

    // This is just here to keep the compiler from complaining.
    // Delete this line when you're starting to work on this function.
    return (Image){(Vector){0, 0}, (Vector){0, 0}, malloc(1)};
}
