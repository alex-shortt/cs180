#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_rotation(Vector3f axis, float angle)
{
    float rad_angle = angle * (float) MY_PI / 180.0;

    Eigen::Matrix3f model = Eigen::Matrix3f::Identity();
    Eigen::Matrix3f N = Eigen::Matrix3f::Identity();

    float x = axis[0];
    float y = axis[1];
    float z = axis[2];

    N << 0, -z, y, 
         z, 0, -x,
         -y, x, 0;

    model = cos(rad_angle) * Eigen::Matrix3f::Identity();
    model += (1.0 - cos(rad_angle)) * axis * axis.transpose();
    model += sin(rad_angle) * N;

    Eigen::Matrix4f matrix = Eigen::Matrix4f::Identity();
    matrix << model(0, 0), model(0, 1), model(0, 2), 0,
              model(1, 0), model(1, 1), model(1, 2), 0,
              model(2, 0), model(2, 1), model(2, 2), 0,
              0, 0, 0, 1;

    return matrix;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    float rad_angle = rotation_angle * (float) MY_PI / 180.0;

    model << cos(rad_angle), -sin(rad_angle), 0, 0, 
             sin(rad_angle), cos(rad_angle), 0, 0,
             0, 0, 1, 0, 
             0, 0, 0, 1;

    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    Eigen::Matrix4f persp_to_ortho = Eigen::Matrix4f::Identity();

    float n = zNear;
    float f = zFar;
    float t = abs(n) * tan(eye_fov / 2.0);
    float r = t * aspect_ratio;
    float l = -r;
    float b = -t;

    persp_to_ortho << n, 0, 0, 0, 
                      0, n, 0, 0, 
                      0, 0, n+f, -n*f, 
                      0, 0, 0, 1;

    return persp_to_ortho;
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);


    Eigen::Vector3f rotation_axis = {1, 1, 0};
    rotation_axis.normalize();

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_rotation(rotation_axis, angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_rotation(rotation_axis, angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
