#include "first_challenge_sample/first_challenge_sample.h"

FirstChallenge::FirstChallenge():private_nh_("~")
{
    private_nh_.param("hz", hz_, {10});
    sub_odom_ = nh_.subscribe("/roomba/odometry", 100, &FirstChallenge::odometry_callback, this);
    sub_laser_ = nh_.subscribe("/scan", 100, &FirstChallenge::laser_callback, this);
    pub_cmd_vel_ = nh_.advertise<roomba_500driver_meiji::RoombaCtrl>("/roomba/control", 1);
}

void FirstChallenge::odometry_callback(const nav_msgs::Odometry::ConstPtr& msg)
{
    odometry_ = *msg;
}

void FirstChallenge::laser_callback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    laser_ = *msg;
}

void FirstChallenge::run()
{
    cmd_vel_.mode = 11;
    cmd_vel_.cntl.linear.x = 0.2;
    cmd_vel_.cntl.angular.z = M_PI/6;

    pub_cmd_vel_.publish(cmd_vel_);
}

void FirstChallenge::process()
{
    ros::Rate loop_late(hz_);
    while(ros::ok())
    {
        run();
        ros::spinOnce();
        loop_late.sleep();
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "first_challenge_sample");
    FirstChallenge first_challenge;
    first_challenge.process();
    return 0;
}
