//
//  character.cpp
//  ocr
//
//  Created by zlh on 3/27/15.
//  Copyright (c) 2015 zlh. All rights reserved.
//

#include "character.h"
#include <iostream>
using namespace std;

Character::Character(unsigned char (*img)[256], int ulr, int ulc, int h, int w ){
    image = img;
    ul_r = ulr;
    ul_c = ulc;
    height = h;
    width = w;
}

Character::~Character(){
    
}

int Character::calc_perimeter(){
    int b_cnt = 0;
    for (int i = ul_r; i <= ul_r + height; i++){
        for (int j = ul_c; j <= ul_c + width; j++){
            if (image[i][j] == 0){
                b_cnt++;
            }
        }
    }
    perimeter = b_cnt;
    return 0;
}

int Character::calc_bit_quads(){
    
    /** q1 bwww*/
    int q1 = 0, q3 = 0, qd = 0, q0 = 0, q2 = 0, q4 = 0;
    
    for(int i = ul_r - 1; i <= ul_r + height + 1; i++){
        for(int j = ul_c - 1; j <= ul_c + width + 1; j++){
            
            int b_count = 0;
            // calculate the 4 neighbors
            for (int k = i; k <= i+1; k++){
                for (int l = j; l <= j+1; l++){
                    if (image[k][l] == 0){
                        b_count++;
                    }
                }
            }
            // judge q1, q3, qd
            if (b_count == 1){
                q1++;
            }else if (b_count == 3){
                q3++;
            }else if (b_count == 2){
                if ((image[i][j] == 0 && image[i+1][j+1] == 0)
                    || (image[i][j+1] == 0 && image[i+1][j] == 0)){
                    qd++;
                }else{
                    q2++;
                }
            }else if (b_count == 0){
                q0++;
            }else if (b_count == 4){
                q4 ++;
            }
        }
    }
    
    bq0 = q0;
    bq1 = q1;
    bq2 = q2;
    bq3 = q3;
    bq4 = q4;
    bqd = qd;
    
    euler = 0.25 * (q1 - q3 - 2 * qd);
    return 0;
}

int Character::calc_euler_number(){
    
    /** figures 0 to 9 have only 1 component */
//    euler = 1 - Character::calc_bit_quads();
    return 0;
}

int Character::calc_sym(){
    /****************************************************/
	int vsy=0,hsy=0;
	for(int i=0;i<=height;i++){
        //whether pixels in the left half are a mirror image to the right half
		for(int j=0;j<=(width+1)/2;j++){
			if(image[ul_r+i][ul_c+j]==image[ul_r+i][ul_c+width-j-1])
				hsy+=2;
		}
	}
	if(0==width%2)
		hsy-=height;
	for(int i=0;i<=(height+1)/2;i++){
        ////whether pixels in the upper half are a mirror image to the lower half
		for(int j=0;j<=width;j++){
			if(image[ul_r+i][ul_c+j]==image[ul_r+height-i-1][ul_c+j])
				vsy+=2;
		}
	}
	if(1==height%2)
		vsy-=width;
	v_sym_rate=double(vsy)/((double)(height+1)*(width+1));
	h_sym_rate=double(hsy)/((double)(height+1)*(width+1));
    return 0;
}

int Character::calc_aspect_ratio(){
    aspect_ratio = (height+ 1 +0.0) / (width + 1);
    return 0;
}

int Character::calc_center(){
    /** Vertical centroid */
    int v_black_cnt = 0;
    int black_cnt = 0;
    for (int i = ul_r; i <= ul_r + height; i++){
        for (int j = ul_c; j <= ul_c + width; j++){
            if (image[i][j] == 0){
                v_black_cnt += (i - ul_r);
                black_cnt ++;
            }
        }
    }
    vertical_center = (double)v_black_cnt / black_cnt;
    
    /** Horizontal centroid */
    int h_black_cnt = 0;
    black_cnt = 0;
    for (int i = ul_r; i <= ul_r + height; i++){
        for (int j = ul_c; j <= ul_c + width; j++){
            if (image[i][j] == 0){
                h_black_cnt += (j - ul_c);
                black_cnt ++;
            }
        }
    }
    horizontal_center = (double)h_black_cnt / black_cnt;
    return 0;
}

void Character::perform_tests(){
    
    Character::calc_perimeter();
    Character::calc_bit_quads();
    Character::calc_center();
    Character::calc_sym();
    Character::calc_aspect_ratio();
    
    return;
}

void Character::classify(){
    
    /** must be 8 */
    if (euler == -1){
        recognized_char = '8';
        return;
    }
    /** maybe 0, 4, 6, 9 */
    if (euler == 0){
        // perfect symmetry, should be 0
        if (v_sym_rate > 0.9 && h_sym_rate > 0.9){
            recognized_char = '0';
            return;
        }else if (width - horizontal_center*2 > 2){    // maybe 6
            recognized_char = '6';
            return;
        }else if (horizontal_center*2 - width > 2){    // maybe 9
            recognized_char = '9';
            return;
        }else{
            recognized_char = '4';
            return;
        }
        
    }
    /** maybe 1, 2, 3, 5, 7 */
    if (euler == 1){
        // better symmetry, should be 1
        if (v_sym_rate > 0.8 && h_sym_rate > 0.8){
            recognized_char = '1';
            return;
        // good vertical symmetry, should be 3
        }if (v_sym_rate > 0.8 && h_sym_rate <=0.8){
            recognized_char = '3';
            return;
        }
        // pixels focus on the upper side, should be 7
        if (height - vertical_center*2 > 8){
            recognized_char = '7';
            return;
        }
        // pixels focus on the upper side, should be 5
        if (height > 2*vertical_center){
            recognized_char = '5';
            return;
        }
        // pixels focus on the down side, should be 2
        if (height < 2*vertical_center){
            recognized_char = '2';
            return;
        }
    }
   
    
    return;
}

char Character::get_classification(){
    return recognized_char;
}

void Character::print_calculations(){
    cout << "Character in bounding box:" << ul_r <<", "
        <<ul_c << " to " << ul_r+height <<", "<<ul_c+width << endl;
    cout<<"====================================" << endl;
    
    cout<<"BQ0="<<bq0<<" "<<"BQ1="<<bq1<<" "<<"BQ2="<<bq2<<" "<<"BQ3="
        <<bq3<<" "<<"BQ4="<<bq4<<" "<<"BQD="<<bqd<<endl;
    
    cout<<"Euler number="<<euler<<endl;
    
    cout<<"height="<<height<<" "<<"width="<<width<<endl;
    
    cout<<"VCentroid="<<vertical_center<<" HCentroid="<<horizontal_center<<endl;
    
    cout<<"VSym="<<v_sym_rate<<" "<<"HSym="<<h_sym_rate<<endl;
    
    cout<<"Aspect ratio="<<aspect_ratio<<endl;
    
}