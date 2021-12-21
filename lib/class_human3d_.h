#pragma once

// std
#include <opencv2/opencv.hpp>
#include "class_detector.h"
#include "class_hmr.h"
#include "class_keypoints.h"
#include "class_tracker.h"

class _Human3d
{
public:
    _Human3d();
    ~_Human3d();

public:
    bool init(const std::string &detect_engine_path_,
              const std::string &kp_engine_path_,
              float kp_conf_,
              const cv::Size &img_size_, //(width, height)
              const std::string &hmr_engine_path_,
              std::string &smpl_male_json_path_);

    void run(const cv::Mat &img_, 
             cv::Rect &person_rect_,
             std::vector<cv::Point2f> &kp_coords_,
             std::vector<float> &kp_scores_,
             std::vector<cv::Vec3f> &vertices_);
    
private:
    bool find_max_rect(const std::vector<cv::Rect> &person_rects_,
                    cv::Rect &max_rect_);
    void output(const cv::Mat &img_,
                    const cv::Rect &target_rect_,
                    cv::Rect &person_rect_,
                    std::vector<cv::Point2f> &kp_coords_,
                    std::vector<float> &kp_scores_,
                    std::vector<cv::Vec3f> &vertices_);
private:
    float _kp_conf;
    cv::Size _img_size;
    Detector _detector;
    Keypoints _keypoints;
    HMR _hmr;
    bool _track_mode;
    Tracker _tracker;
    int _target_id;
};
