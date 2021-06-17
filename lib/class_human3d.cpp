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
              const std::string &hmr_engine_path_,
              std::string &smpl_male_json_path_)
{
    return _impl->_human3d.init(detect_engine_path_, 
                                hmr_engine_path_, 
                                smpl_male_json_path_);
}

bool Human3d::run(const cv::Mat &img_, std::vector<cv::Vec3f> &vertices_)
{
    return _impl->_human3d.run(img_, vertices_);
}


