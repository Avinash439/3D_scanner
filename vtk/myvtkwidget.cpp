#include "myvtkwidget.h"

MyVTKWidget::MyVTKWidget(QWidget *parent) : QWidget(parent)
{
    //initialize widgets and layouts
    centralWidget_ = new QWidget(this);
    sideContainer = new QWidget(this);
    buttonContainer = new QWidget(this);
    mainLayout_ = new QHBoxLayout;
    sideLayout_ = new QVBoxLayout;
    buttonLayout = new QVBoxLayout;

    //Initialize Progress Bar
    progressBar = new QProgressBar(this);

    //Intialize VTK Widget
    //startButton_ = new QPushButton(this);
    displayWidget_ = new QVTKWidget;

    //Initialize startButton
    readButton_ = new QPushButton("Read", this);
    scanButton = new QPushButton("Scan", this);

    //Initialize the cloud list widget and connect it to the slot
    cloudList = new QListWidget(this);
    QObject::connect(cloudList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(on_cloudListItem_clicked(QListWidgetItem*)));

    //Connect the pushButton pressed signal to its slot
    QObject::connect(readButton_, SIGNAL(pressed()), this, SLOT(on_readButton_pressed()));
    QObject::connect(scanButton, SIGNAL(pressed()), this, SLOT(on_scanButton_pressed()));

    //initialize point cloud
    //cloud.reset(new pcl::PointCloud<PointT>);

    /*qDebug() << "Calling get count";
    count = fileGrab.getCount();

    for(int i=0; i < count; i++)
    {

        cloudVector.push_back(cloud);
    }*/

    //initialize PCL Viewer
    m_visualizer = new pcl::visualization::PCLVisualizer ("3D Viewer",false);
    m_visualizer->setBackgroundColor(0.1,0.1,0.1);

    //renderWindow = m_visualizer->getRenderWindow();
    displayWidget_->SetRenderWindow(m_visualizer->getRenderWindow());
    m_visualizer->setupInteractor(displayWidget_->GetInteractor(), displayWidget_->GetRenderWindow());
}

QWidget* MyVTKWidget::getCentralWidget()
{
    //get the screen resolution
    ScreenSetup screenSetup;
    QSize size = screenSetup.size();
    int x = size.width();
    int y = size.height();

    //set the dimensions and positions for widgets
    //displayWidget_->setFixedSize(x,(3*(y/4)));
    //displayWidget_->setGeometry((x/4),0,x,y);
    //startButton_->setFixedSize(x,(y/4));
    //sideContainer->setGeometry(0,0, (x/4), y);
    sideContainer->setFixedWidth((x/4));

    //setup central widget
    mainLayout_->setSpacing(5);
    mainLayout_->setContentsMargins(5,0,5,0);

    buttonLayout->setSpacing(5);
    sideLayout_->setSpacing(5);
    sideLayout_->setContentsMargins(5,5,5,5);

    buttonLayout->addWidget(readButton_);
    buttonLayout->addWidget(scanButton);
    buttonContainer->setLayout(buttonLayout);

    sideLayout_->addWidget(buttonContainer);
    sideLayout_->addWidget(cloudList);
    sideLayout_->addWidget(progressBar);
    sideContainer->setLayout(sideLayout_);

    mainLayout_->addWidget(sideContainer);
    mainLayout_->addWidget(displayWidget_);
    centralWidget_->setLayout(mainLayout_);

    return centralWidget_;
}

void MyVTKWidget::on_readButton_pressed()
{
    //Read and display the point cloud

    fileGrab.initializeFileList();
    pcdNames = fileGrab.getNames();

    //access the names
    vector<QString>::iterator it = pcdNames.begin();
    while(it != pcdNames.end())
    {
        cloudList->addItem(*it);
        it++;
    }

    cloudVector = fileGrab.getPointClouds();

    //Display the first point cloud in the list
    m_visualizer->addPointCloud(cloudVector[0], "ccc");
    m_visualizer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "ccc");
    m_visualizer->addCoordinateSystem();
    displayWidget_->update();
    /*
    pcl::io::loadPCDFile(path, *cloud);
    qDebug() << cloud->points.size();
    m_visualizer->addPointCloud(cloud, "ccc");
    m_visualizer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "ccc");
    m_visualizer->addCoordinateSystem();
    displayWidget_->update();*/
}

void MyVTKWidget::on_cloudListItem_clicked(QListWidgetItem *item)
{
    //qDebug() << "Putain";
    //qDebug() << count;

    for(int i = 1; i <= cloudVector.size(); i++)
    {
        //qDebug() << "Jagjit Singh";

        if(cloudList->item(i) == item)
        {
            //qDebug() << "more putain";
            m_visualizer->removePointCloud("ccc");
            m_visualizer->addPointCloud(cloudVector[i-1], "ccc");
            //m_visualizer->spinOnce();
            displayWidget_->update();
            qDebug() << "point cloud updated" << i;
        }
    }
}

void MyVTKWidget::on_scanButton_pressed()
{

}
