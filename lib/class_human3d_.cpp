#include "class_human3d_.h"

_Human3d::_Human3d() {}
_Human3d::~_Human3d() 
{
    
}

bool _Human3d::init(const std::string &detect_engine_path_,
                    const std::string &kp_engine_path_,
                    float kp_conf_,
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
    {
        bool is_init = _keypoints.init(kp_engine_path_);
        if (!is_init)
        {
            return false;
        }
    }

    _kp_conf = kp_conf_;

    return true;
}

void _Human3d::run(const cv::Mat &img_, 
                    cv::Rect &person_rect_,
                    std::vector<cv::Point2f> &kp_coords_,
                    std::vector<float> &kp_scores_,
                    std::vector<cv::Vec3f> &vertices_)
{
    //
    person_rect_ = cv::Rect(0,0,0,0);
    kp_coords_.clear();
    kp_scores_.clear();
    vertices_.clear();

    //
    std::vector<cv::Mat> imgs;
    imgs.push_back(img_);

    std::vector<std::vector<Detection> > vec_detections;
    _detector.detect(imgs, vec_detections);

    if (0 == vec_detections.size())
    {
        return;
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
        person_rect_ = max_area_detection.rect;

        std::vector<cv::Mat> imgs;
        imgs.push_back(img_(max_area_detection.rect));

        // kp
        std::vector<std::vector<cv::Point2f> > vec_coords;
        std::vector<std::vector<float> > vec_scores;
        bool is_run = _keypoints.run(imgs, vec_coords, vec_scores);
        if ((!is_run) || (0 == vec_scores.size()))
        {
            return;
        }
        kp_coords_ = vec_coords[0];
        float offset_x = max_area_detection.rect.x;
        float offset_y = max_area_detection.rect.y;

        for (int i = 0; i < kp_coords_.size(); i++)
        {
            kp_coords_[i] += cv::Point2f(offset_x, offset_y);
        }

        kp_scores_ = vec_scores[0];

        bool kp_unvalid = true;
        if (kp_scores_[15] > _kp_conf && kp_scores_[16] > _kp_conf) // ankles are visible
        {
            kp_unvalid = false;
        }

        if (kp_unvalid)
        {
            return;
        }

        // hmr
        std::vector<std::vector<cv::Vec3f> > poses;
        std::vector<std::vector<float> > shapes;
        std::vector<std::vector<cv::Vec3f> > vec_3djoints;
        std::vector<std::vector<cv::Vec3f> > vec_vertices;
        is_run = _hmr.run_joints(imgs, poses, shapes, vec_3djoints, vec_vertices);
        if ((!is_run) || (0 == vec_vertices.size()))
        {
            return;
        }

        vertices_ = vec_vertices[0];
    }
    
}

