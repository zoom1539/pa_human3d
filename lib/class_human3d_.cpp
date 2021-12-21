#include "class_human3d_.h"

_Human3d::_Human3d() 
{
    _track_mode = false;
}
_Human3d::~_Human3d() 
{
    
}

bool _Human3d::init(const std::string &detect_engine_path_,
                    const std::string &kp_engine_path_,
                    float kp_conf_,
                    const cv::Size &img_size_, //(width, height)
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
    _img_size = img_size_;

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

    std::vector<cv::Rect> person_rects;
    for (int i = 0; i < vec_detections[0].size(); i++)
    {
        if (vec_detections[0][i].class_id == 0)
        {
            person_rects.push_back(vec_detections[0][i].rect);
        }
    }

    if (0 == person_rects.size())
    {
        return;
    }

    //
    bool has_target = false;
    cv::Rect target_rect;
    if (!_track_mode)
    {
        cv::Rect max_rect;
        has_target = find_max_rect(person_rects, max_rect);
        target_rect = max_rect;
        _track_mode = true;

        //
        std::vector<cv::Rect> rects;
        rects.push_back(max_rect);
        std::vector<TrackedBox> rects_tracked;
		_tracker.run(rects, _img_size, rects_tracked);

        if (rects_tracked.size() > 0)
        {
            _target_id = rects_tracked[0].id;
        }

    }
    else
    {
        std::vector<TrackedBox> rects_tracked;
		_tracker.run(person_rects, _img_size, rects_tracked);

        for (int i = 0; i < rects_tracked.size(); i++)
		{
			if (rects_tracked[i].id == _target_id)
            {
                target_rect = rects_tracked[i].rect;
                has_target = true;
                break;
            }
		}

        if (!has_target)
        {
            _track_mode = false;
        }

    }

    
    if (has_target)
    {
        output(img_, target_rect, 
               person_rect_, kp_coords_, kp_scores_, vertices_);
    }
    
}

void _Human3d::output(const cv::Mat &img_,
                    const cv::Rect &target_rect_,
                    cv::Rect &person_rect_,
                    std::vector<cv::Point2f> &kp_coords_,
                    std::vector<float> &kp_scores_,
                    std::vector<cv::Vec3f> &vertices_)
{
    person_rect_ = target_rect_;

    std::vector<cv::Mat> imgs;
    imgs.push_back(img_(target_rect_));

    // kp
    std::vector<std::vector<cv::Point2f> > vec_coords;
    std::vector<std::vector<float> > vec_scores;
    bool is_run = _keypoints.run(imgs, vec_coords, vec_scores);
    if ((!is_run) || (0 == vec_scores.size()))
    {
        return;
    }
    kp_coords_ = vec_coords[0];
    float offset_x = target_rect_.x;
    float offset_y = target_rect_.y;

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

bool _Human3d::find_max_rect(const std::vector<cv::Rect> &person_rects_,
                    cv::Rect &max_rect_)
{
    cv::Rect max_rect = cv::Rect(0, 0, 0, 0);
    for (int i = 0; i < person_rects_.size(); i++)
    {
        if (person_rects_[i].area() > max_rect.area())
        {
            max_rect = person_rects_[i]; 
        }
    }

    max_rect_ = max_rect;
    return true;
}

