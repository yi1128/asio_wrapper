#pragma once
#include<iostream>


class asio_wrapper
{
public:
	asio_wrapper();
	
	/// <summary>
	/// Initialize of Network
	/// IP(std::string), PORT(int)
	/// </summary>
	/// <returns></returns>
	virtual bool Init(std::string ip_, int port_) = 0;
	
	/*
	* bumper_obstacle_pub_ = node_->advertise<sensor_msgs::PointCloud2>(std::string("/obstacle/bumper"), 10);
	* bumper_obstacle_pub_.publish(bumper_pointcloud_obstacle_clustered);
	*/
	virtual bool Send() = 0;
	/*
	* roof_left_lidar_sub_ = node_->subscribe<sensor_msgs::LaserScan>("/laser/roof_left", 10,
                                                                        &CObstacleDetection::RoofLeftLidarCallback,

    void RoofLeftLidarCallback(const sensor_msgs::LaserScanConstPtr &msg)
    {

        PointCloud2ptr pointcloud_obstacle(new sensor_msgs::PointCloud2);

        LaserScanToObstaclePointCloud(msg, pointcloud_obstacle);
        pointcloud_roof_left_obs_ = pointcloud_obstacle;

        // roof_left_obstacle_pub_.publish(pointcloud_obstacle);


        pcl::PointCloud<pcl::PointXYZI>::Ptr point_xyz_left_obstacle(new pcl::PointCloud<pcl::PointXYZI>);
        pcl::fromROSMsg(*pointcloud_obstacle, *point_xyz_left_obstacle);

        pcl::PointCloud<pcl::PointXYZI> filtering_xyz_left_obstacle;
        pcl::PointCloud<pcl::PointXYZI> filtering_xyz_left_obstacle_3M;

        if(running == SPEEDIUM)
            Cloud_segmentation(1,pointcloud_obstacle,*point_xyz_left_obstacle,filtering_xyz_left_obstacle,filtering_xyz_left_obstacle_3M);
        else
            Cloud_segmentation_PRACTICE(1,pointcloud_obstacle,*point_xyz_left_obstacle,filtering_xyz_left_obstacle,filtering_xyz_left_obstacle_3M);

        pcl::PointCloud<pcl::PointXYZI> clustered_xyz_left_obstacle;


//        Cloud_segmentation(roof_left_obs,*point_xyz_left_obstacle,clustered_xyz_left_obstacle);

        Making_Clustering(1,pointcloud_obstacle,filtering_xyz_left_obstacle,filtering_xyz_left_obstacle_3M,clustered_xyz_left_obstacle);
        point_xyz_left_obstacle->clear();


        // roof left clustered pointcloud
        sensor_msgs::PointCloud2 left_pointcloud_obstacle_clustered;
        pcl::toROSMsg(clustered_xyz_left_obstacle, left_pointcloud_obstacle_clustered);

        left_pointcloud_obstacle_clustered.header.frame_id = grandparent_frame_id;
        left_pointcloud_obstacle_clustered.header.stamp = pointcloud_obstacle->header.stamp;
        roof_left_obstacle_pub_.publish(left_pointcloud_obstacle_clustered);

        clustered_xyz_left_obstacle.clear();


    }
	*/
	virtual bool Recv() = 0;

	virtual ~asio_wrapper() = 0;
	
private:

	

};

class asio_TCP_server
{
public:
	
private:

};

class asio_TCP_client
{
public:

private:

};