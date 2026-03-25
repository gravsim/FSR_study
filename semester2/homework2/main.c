#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include "lodepng.h" 


#define DARK 10
#define BRIGHT 135

unsigned char* load_png(const char* filename, unsigned int* width, unsigned int* height)  {
    unsigned char* image = NULL;
    int error = lodepng_decode32_file(&image, width, height, filename);
    if (error != 0) {
        printf("error %u: %s\n", error, lodepng_error_text(error));
    }
    return image;
}


void write_png(const char* filename, const unsigned char* image, unsigned width, unsigned height) {
    unsigned char* png;
    long long unsigned int pngsize;
    int error = lodepng_encode32(&png, &pngsize, image, width, height);
    if (error == 0) {
        lodepng_save_file(png, pngsize, filename);
    } else {
        printf("error %u: %s\n", error, lodepng_error_text(error));
    }
    free(png);
}


int check_pixel(unsigned char *picture, int bw_size, int i, int neighbour) {
    if (!picture) {
        return -1;
    }
    if (i >= 0 && i < bw_size && neighbour > 0 && neighbour < bw_size) {
        if (picture[i] - picture[neighbour] > 45) {
            return 1;
        }
    }
    return 0;
}


int turn_back(unsigned char *picture, unsigned char *bw_pic, int bw_size) {
    if (!picture || !bw_pic) {
        return -1;
    }
    int i;
    for (i = 0; i < bw_size; i++) {
        picture[i * 4] = bw_pic[i];
        picture[i * 4 + 1] = bw_pic[i];
        picture[i * 4 + 2] = bw_pic[i];
        picture[i * 4 + 3] = 255;
    }
    return 1;
}


int fill_rectangles(unsigned char* bw_pic, unsigned char* territory, int width) {
    if (!territory) {
        return -1;
    }
    int x;
    int y;
    int sector;
    int sectors_amount = 9;
    /*
     Координаты правого верхнего и левого нижнего углов прямоугольников,
     в которых находятся танкеры. Эти координаты я посчитал в фотошопе, прямоугольники,
     которые я обозначил можно посмотреть на картинке territory.png.
    */
    int sectors[9][2][2] = {
        {
            {288, 12},
            {356, 121}
        },
        {
                {597, 288},
                {1117, 646}
        },
        {
                {588, 0},
                {596, 600}
        },
        {
                {597, 0},
                {716, 287}
        },
        {
                {536, 327},
                {557, 556}
        },
        {
                {503, 328},
                {535, 343}
        },
        {
                {511, 344},
                {535, 403}
        },
        {
                {551, 292},
                {557, 326}
        },
        {
                {507, 292},
                {550, 315}
        }
    };
    for (sector = 0; sector < sectors_amount; sector++) {
        for (x = sectors[sector][0][0]; x < sectors[sector][1][0]; x++) {
            for (y = sectors[sector][0][1]; y < sectors[sector][1][1]; y++) {
                if (bw_pic[y * width + x]) {
                    territory[y * width + x] = 1;
                }
            }
        }
    }
    return 1;
}


int DFS_recursive(
    unsigned char* bw_pic
    , unsigned char* visited
    , int current
    , int bw_size
    , unsigned char* territory
    , int width
    ) {
    if (!bw_pic
        ||
        !visited
        ||
        current < 0
        ||
        current >= bw_size
        ||
        visited[current]
        ||
        !territory[current]
        ||
        !bw_pic[current]
        ) {
        return -1;
    }
    visited[current] = 1;
    DFS_recursive(bw_pic, visited, current + 1, bw_size, territory, width);
    DFS_recursive(bw_pic, visited, current - 1, bw_size, territory, width);
    DFS_recursive(bw_pic, visited, current + width, bw_size, territory, width);
    DFS_recursive(bw_pic, visited, current - width, bw_size, territory, width);
    return 0;
}


int set_contrast(int bw_size, unsigned char* bw_pic, unsigned char* picture, int width) {
    if (!bw_pic || !picture) {
        return -1;
    }
    int i;
    unsigned char value;
    for (i = 0; i < bw_size; i++) {
        value = (picture[i * 4] + picture[i * 4 + 1] + picture[i * 4 + 2]) / 3;
        bw_pic[i] = value;
    }
    for (i = 0; i < bw_size; i++) {
        if (
            check_pixel(bw_pic, bw_size, i, i - 1)
            || check_pixel(bw_pic, bw_size, i, i + 1)
            || check_pixel(bw_pic, bw_size, i, i - width)
            || check_pixel(bw_pic, bw_size, i , i + width)
        ) {
            bw_pic[i] = 255;
        }
    }
    for (i = 0; i < bw_size; i++) {
        if (bw_pic[i] < 255) {
            bw_pic[i] = 0;
        }
    }
    return 1;
}


int main() {
    const char* filename = "photo_2026-03-05_12-50-07.png";
    unsigned int width, height;
    int bw_size;
    unsigned char* picture = load_png(filename, &width, &height);
    int answer = 0;
    int i;
    if (picture == NULL) {
        printf("Problem reading picture from the file %s. Error.\n", filename); 
        return -1; 
    }
    bw_size = width * height;
    unsigned char* bw_pic = calloc(bw_size, sizeof(unsigned char));
    unsigned char* visited = calloc(bw_size, sizeof(unsigned char));
    unsigned char* territory = calloc(bw_size, sizeof(unsigned char));
    set_contrast(bw_size, bw_pic, picture, width);
    turn_back(picture, bw_pic, bw_size);
    write_png("contrast.png", picture, width, height);
    fill_rectangles(bw_pic, territory, width);
    for (i = 0; i < bw_size; i++) {
        if (!visited[i] && territory[i]) {
            DFS_recursive(territory, visited, i, bw_size, territory, width);
            answer++;
        }
    }
    turn_back(picture, territory, bw_size);
    write_png("only_tankers.png", picture, width, height);
    printf("Amount of tankers: %d", answer);
    free(bw_pic); 
    free(territory);
    free(visited);
    free(picture);
    return 0; 
}
