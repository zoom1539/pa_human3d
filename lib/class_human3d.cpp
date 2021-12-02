#include "class_human3d.h"
#include "class_human3d_.h"

class Human3d::Impl
{
public:
    _Human3d _human3d;
};

Human3d::Human3d() : _impl(new Human3d::Impl())
{
}

Human3d::~Human3d()
{
    delete _impl;
    _impl = NULL;
}

bool Human3d::init(const std::string &detect_engine_path_,
                    const std::string &kp_engine_path_,
                    float kp_conf_,
                    const std::string &hmr_engine_path_,
                    std::string &smpl_male_json_path_)
{
    return _impl->_human3d.init(detect_engine_path_, 
                                kp_engine_path_,
                                kp_conf_,
                                hmr_engine_path_, 
                                smpl_male_json_path_);
}

void Human3d::run(const cv::Mat &img_, 
             cv::Rect &person_rect_,
             std::vector<cv::Point2f> &kp_coords_,
             std::vector<float> &kp_scores_,
             std::vector<cv::Vec3f> &vertices_)
{
    return _impl->_human3d.run(img_, person_rect_, kp_coords_, kp_scores_, vertices_);
}


