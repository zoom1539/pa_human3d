#pragma once

// std
#include <opencv2/opencv.hpp>
#include "class_detector.h"
#include "class_hmr.h"

class _Human3d
{
public:
    _Human3d();
    ~_Human3d();

public:
    bool init(const std::string &detect_engine_path_,
              const std::string &hmr_engine_path_,
              std::string &smpl_male_json_path_);

    bool run(const cv::Mat &img_, std::vector<cv::Vec3f> &vertices_);
    
private:
    Detector _detector;
    HMR _hmr;
};
