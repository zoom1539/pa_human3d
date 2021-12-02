#pragma once

#include "opencv2/opencv.hpp"

class Human3d
{
public:
    explicit Human3d();
    ~Human3d();

    bool init(const std::string &detect_engine_path_,
              const std::string &kp_engine_path_,
              float kp_conf_,
              const std::string &hmr_engine_path_,
              std::string &smpl_male_json_path_);

    void run(const cv::Mat &img_, 
             cv::Rect &person_rect_,
             std::vector<cv::Point2f> &kp_coords_,
             std::vector<float> &kp_scores_,
             std::vector<cv::Vec3f> &vertices_);

private:
    Human3d(const Human3d &);
    const Human3d &operator=(const Human3d &);

    class Impl;
    Impl *_impl;
};
