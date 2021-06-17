#pragma once

#include "opencv2/opencv.hpp"

typedef struct _Detection 
{
    cv::Rect rect;
    float conf;  // bbox_conf * cls_conf
    float class_id;
}Detection;

class Detector
{
public:
    explicit Detector();
    ~Detector();

    bool serialize(std::string &wts_path_, const std::string &engine_path_);

    bool init(const std::string &engine_path_);

    bool detect(const std::vector<cv::Mat> &imgs_, std::vector<std::vector<Detection> > &vec_detections_);
    

private:
    Detector(const Detector &);
    const Detector &operator=(const Detector &);

    class Impl;
    Impl *_impl;
};
