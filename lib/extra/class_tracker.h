#pragma once

#include "opencv2/opencv.hpp"

typedef struct _TrackedBox
{
	int id;
	cv::Rect rect;
}TrackedBox;

class Tracker
{
public:
    explicit Tracker();
    ~Tracker();

    // sort
    bool run(const std::vector<cv::Rect> &rects_, 
             const cv::Size &img_size_, //(width, height) 
             std::vector<TrackedBox> &rects_tracked_);

    // deep sort
    bool run(const std::vector<cv::Rect> &rects_, 
             const std::vector<cv::Mat> &features_,
             std::vector<TrackedBox> &rects_tracked_);
    
    

private:
    Tracker(const Tracker &);
    const Tracker &operator=(const Tracker &);

    class Impl;
    Impl *_impl;
};
