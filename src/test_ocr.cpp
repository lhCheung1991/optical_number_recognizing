#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <cstdlib>
#include "character.h"
#include "point.h"
#include "bmplib.h"
#include <string.h>

using namespace std;
// /Users/lhcheung1991/Desktop/business/20150326_ocr_c++_300/ocr_images/ocr1.bmp
// /Users/lhcheung1991/Desktop/business/20150326_ocr_c++_300/ocr_images/ocr2.bmp
// /Users/lhcheung1991/Desktop/business/20150326_ocr_c++_300/ocr_images/ocr3.bmp
#define THRESH 128

unsigned char image[SIZE][SIZE];		// Input Image Data Array
unsigned char explored[SIZE][SIZE];		// 
int visited[SIZE][SIZE];
const int BLACK = 0;
vector<Character> vec;

Character bfs(unsigned char (*img)[SIZE], int row, int column);

int main(int argc, char *argv[])
{
    cout << "Pls input the image file path:"<<endl;
    string str_img_file;
    cin >> str_img_file;
    /** read the image */
    char image_file[200];
    strcpy(image_file, str_img_file.c_str());
    int deg = 0;
    cout << "Pls input the debug info mode (1/0):"<<endl;
    cin >> deg;
    
    readGSBMP(image_file, image);
    
    /** denoise the image */
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            // threshold is 64
            if (image[i][j] <= THRESH){
                image[i][j] = 0;
            }else{
                image[i][j] = 255;
            }
        }
    }
    
    /** bfs to find the character */
    /** can not modify the image */
    memset(visited, 0, SIZE*SIZE*sizeof(int));
    //search every black pixel
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            if (image[i][j] == BLACK && visited[i][j] == 0){
                vec.push_back(bfs(image, i, j));
            }
        }
    }
    
    vector<Character>::iterator ite;
    int cnt = 1;
    for (ite = vec.begin(); ite != vec.end(); ite++){
        ite->perform_tests();    // calulate params
        ite->classify();
        if (deg == 1){
            ite->print_calculations();
        }
        
        cout<< "Character "<< cnt++ <<" is classified as:"<<ite->get_classification() <<endl<<endl;
    }

//    char tmp[100] = "/Users/lhcheung1991/Desktop/business/20150326_ocr_c++_300/ocr_images/ocr11_32.bmp";
//    writeGSBMP(tmp, image);
    
    return 0;
}

 Character bfs(unsigned char (*img)[SIZE], int row, int column){
     
     /** ulr, ulc, lrr lrc*/
     int ulr = 255, ulc = 255, lrr = 0, lrc = 0;
     
     /** deque */
     deque<Point> queue;
     Point p(row, column);
     queue.push_back(p);
     visited[row][column] = 1;
     
     while(!queue.empty()){
         Point *ptr_p = &queue.front();
         queue.pop_front();
         
         /** update the bounding */
         ulr = ulr < ptr_p->r ? ulr : ptr_p->r;
         ulc = ulc < ptr_p->c ? ulc : ptr_p->c;
         lrr = lrr > ptr_p->r ? lrr : ptr_p->r;
         lrc = lrc > ptr_p->c ? lrc : ptr_p->c;
         
         /** search 8 directions */
         for (int i = ptr_p->r-1; i <= ptr_p->r+1; i++){
             for (int j = ptr_p->c-1; j <= ptr_p->c+1; j++){
                 if(img[i][j] == BLACK && visited[i][j] == 0){
                     Point tmp_p(i, j);
                     queue.push_back(tmp_p);
                     visited[i][j] = 1;
                 }
             }
         }
         
     }
     
     /*
     for (int i = ulr; i <= lrr; i++){
         img[i][ulc] = 128;
     }
     for (int i = ulr; i <= lrr; i++){
         img[i][lrc] = 128;
     }
     for (int j = ulc; j <= lrc; j++){
         img[ulr][j] = 128;
     }
     for (int j = ulc; j <= lrc; j++){
         img[lrr][j] = 128;
     }
     */
      
     Character character(img, ulr, ulc, lrr - ulr, lrc - ulc);
     
     return character;
}

