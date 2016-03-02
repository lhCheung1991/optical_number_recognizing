#ifndef CHARACTER_H
#define CHARACTER_H

class Character {
 public:
    Character();
    Character(unsigned char (*img)[256], int ulr, int ulc, int h, int w );
    ~Character();

    void init(int r, int c);
    void perform_tests();
    void classify();
    char get_classification();
    int get_bounding_box(int &sr, int &sc, int &er, int &ec);
    void print_calculations();

 private:

    int calc_bit_quads(); //
    int calc_area();
    int calc_perimeter();
    int calc_euler_number(); //
    int calc_circularity();
    int calc_spatial_moment();
    int calc_sym(); //
    int calc_aspect_ratio(); //
    
    int calc_center();

    int ul_r, ul_c;
    int height, width;

  //  int North, South, East, West;
    int bq0,bq1,bq2,bq3,bq4,bqd; //
    double vertical_center, horizontal_center;
    int area, perimeter, euler;
    double norm_area, norm_perimeter;
    double sh_vavg, sh_havg;
    double circularity, aspect_ratio;
    double herr_rate, verr_rate;
    
    double v_sym_rate, h_sym_rate; //

  
    char recognized_char;
    unsigned char (*image)[256];


};

#endif

