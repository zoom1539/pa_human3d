#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <sys/time.h>  
#include "class_human3d.h"

int main()
{
    Human3d human3d;

    //
    std::string detect_engine_path = "../lib/extra/yolov5s_fp16_b1.engine";
    std::string kp_engine_path = "../lib/extra/kp_fp16_b1.engine";
    std::string hmr_engine_path = "../lib/extra/hmr_fp16_b1.engine";
    std::string smpl_male_json_path = "../lib/extra/smpl_male.json";
    float kp_conf = 0.3;
    
    bool is_init = human3d.init(detect_engine_path, 
                                kp_engine_path, 
                                kp_conf,
                                hmr_engine_path,
                                smpl_male_json_path);
    if(!is_init)
    {
        std::cout << "init fail\n";
        return 0;
    }

    //
    int iter = 10;
    double total_time = 0;

    for (int i = 0; i < iter; i++)
    {
        cv::Mat img = cv::imread("../data/zidane.jpg");

        struct timeval tvBegin, tvEnd;
        gettimeofday(&tvBegin, NULL);

        cv::Rect person_rect;
        std::vector<cv::Point2f> kp_coords;
        std::vector<float> kp_scores;
        std::vector<cv::Vec3f> vertices;
        human3d.run(img, person_rect, kp_coords, kp_scores, vertices);
        

        gettimeofday(&tvEnd, NULL);
        double dDuration = 1000 * (tvEnd.tv_sec - tvBegin.tv_sec) + ((tvEnd.tv_usec - tvBegin.tv_usec) / 1000.0);
        total_time += dDuration;
        std::cout << dDuration << " ms" << std::endl;

        // rect
        if (person_rect.width > 0 && person_rect.height > 0)
        {
            cv::rectangle(img, person_rect, cv::Scalar(255, 0, 0),1, cv::LINE_8,0);
            std::cout << "rect draw\n";
        }

        // kp
        if (kp_coords.size() > 0)
        {
            std::vector<cv::Vec2i> bones = {cv::Vec2i(0, 1), cv::Vec2i(0, 2), cv::Vec2i(1, 3), cv::Vec2i(2, 4),
                                            cv::Vec2i(5, 6), cv::Vec2i(5, 7), cv::Vec2i(7, 9), 
                                            cv::Vec2i(6, 8),cv::Vec2i(8, 10),
                                            cv::Vec2i(11, 12), cv::Vec2i(11, 13), cv::Vec2i(13, 15), 
                                            cv::Vec2i(12, 14),cv::Vec2i(12, 16)};
            
            for (int i = 0 ; i < bones.size(); i++)
            {
                if (kp_scores[bones[i][0]] > kp_conf && kp_scores[bones[i][1]] > kp_conf)
                {
                    cv::Point p1(kp_coords[bones[i][0]]);
                    cv::Point p2(kp_coords[bones[i][1]]);
                    cv::line(img, p1, p2, cv::Scalar(0, 0, 255),1,cv::LINE_AA);
                }
            }

            if (kp_scores[0] > kp_conf && kp_scores[5] > kp_conf && kp_scores[6] > kp_conf)
            {
                cv::Point p1(kp_coords[0]);
                cv::Point p2((kp_coords[5] + kp_coords[6]) * 0.5);
                cv::line(img, p1, p2, cv::Scalar(0, 0, 255),1,cv::LINE_AA);
            }
            if (  kp_scores[11] > kp_conf && kp_scores[12] > kp_conf 
                &&kp_scores[5] > kp_conf && kp_scores[6] > kp_conf)
            {
                cv::Point p1((kp_coords[11] + kp_coords[12]) * 0.5);
                cv::Point p2((kp_coords[5] + kp_coords[6]) * 0.5);
                cv::line(img, p1, p2, cv::Scalar(0, 0, 255),1,cv::LINE_AA);
            }

            std::cout << "kp draw\n";

            cv::imwrite("rect.jpg", img);
        }

        // vertice
        if (vertices.size() > 0)
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
    }

    std::cout << "average: " << total_time / iter << " ms" << std::endl;


    std::cin.get();
    return 0;
}

