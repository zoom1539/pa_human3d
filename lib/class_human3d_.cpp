#include "class_human3d_.h"

_Human3d::_Human3d() {}
_Human3d::~_Human3d() 
{
    
}

bool _Human3d::init(const std::string &detect_engine_path_,
                    const std::string &hmr_engine_path_,
                    std::string &smpl_male_json_path_)
{
    {
        bool is_init = _detector.init(detect_engine_path_);
        if (!is_init)
        {
            return false;
        }
    }
    {
        bool is_init = _hmr.init_joints(hmr_engine_path_, smpl_male_json_path_);
        if (!is_init)
        {
            return false;
        }
    }

    return true;
}

bool _Human3d::run(const cv::Mat &img_, std::vector<cv::Vec3f> &vertices_)
{
    std::vector<cv::Mat> imgs;
    imgs.push_back(img_);

    std::vector<std::vector<Detection> > vec_detections;
    _detector.detect(imgs, vec_detections);

    if (0 == vec_detections.size())
    {
        return false;
    }

    bool has_person = false;
    Detection max_area_detection;
    max_area_detection.rect = cv::Rect(0, 0, 0, 0);
    for (int i = 0; i < vec_detections[0].size(); i++)
    {
        if (vec_detections[0][i].class_id == 0)
        {
            has_person = true;
            if (vec_detections[0][i].rect.area() > max_area_detection.rect.area())
            {
               max_area_detection = vec_detections[0][i]; 
            }
        }
    }

    if (has_person)
    {
        std::vector<cv::Mat> imgs;
        imgs.push_back(img_(max_area_detection.rect));

        std::vector<std::vector<cv::Vec3f> > poses;
        std::vector<std::vector<float> > shapes;
        std::vector<std::vector<cv::Vec3f> > vec_3djoints;
        std::vector<std::vector<cv::Vec3f> > vec_vertices;
        bool is_run = _hmr.run_joints(imgs, poses, shapes, vec_3djoints, vec_vertices);
        if ((!is_run) || (0 == vec_vertices.size()))
        {
            return false;
        }

        vertices_ = vec_vertices[0];
        return true;
    }
    else
    {
        return false;
    }
    
}

