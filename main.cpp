#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <chrono>
#include "class_human3d.h"

int main()
{
    Human3d human3d;

    //
    std::string detect_engine_path = "../lib/extra/yolov5s_fp16_b1.engine";
    std::string hmr_engine_path = "../lib/extra/hmr_fp16_b1.engine";
    std::string smpl_male_json_path = "../lib/extra/smpl_male.json";
    
    bool is_init = human3d.init(detect_engine_path, 
                                hmr_engine_path,
                                smpl_male_json_path);
    if(!is_init)
    {
        std::cout << "init fail\n";
        return 0;
    }

    //
    cv::Mat img = cv::imread("../data/im1010.jpg");
    
    auto start = std::chrono::system_clock::now();

    std::vector<cv::Vec3f> vertices;
    bool is_run = human3d.run(img, vertices);
    if(!is_run)
    {
        std::cout << "no people detected\n";
    }

    auto end = std::chrono::system_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " total ms" << std::endl;

    if (is_run)
    {
        std::ofstream outFile("verts.obj");
        
        for (int j = 0; j < vertices.size(); j++)
        {
            outFile <<"v ";
            for (int k = 0; k < 3; k++)
            {
                outFile << vertices[j][k] << " ";
            }
            
            outFile <<"\n";
        }
            
        outFile.close();
        std::cout << "verts save\n";
    }

    std::cin.get();
    return 0;
}

