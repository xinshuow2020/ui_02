#include "FaceLogic.h"
#include <fstream>
#include <math.h>
#include <QDebug>

FacePostureType FaceLogic::rtType =Normal;
vector<Point> FaceLogic::normal =vector<Point>();

bool FaceLogic::isInitialized()
{
    qDebug() << "Enter isInitialized";
	ifstream file("position.txt");
    qDebug() << "Leave isInitialized";
	return file;
}

bool FaceLogic::storeNormalValue(const Point& lfeye,const Point& rteye,const Point& mouth)
{
    qDebug() << "Enter storeNormalValue";
	ofstream out("position.txt");  
    if (out.is_open())   
    {
        qDebug() << "Open position.txt";
        out<<lfeye.x<<" "<<lfeye.y<<endl;
        out<<rteye.x<<" "<<rteye.y<<endl;
        out<<mouth.x<<" "<<mouth.y<<endl;
        out.close();
        return true;
    }
    return false;
}

vector<Point> FaceLogic::getNormalValue()
{
	ifstream file("position.txt");
	vector<Point> temp;
	if(file.is_open())
	{
		string line;
		while(getline(file,line))
		{
			int index = line.find_first_of(" ");
			string x =line.substr(0,index);
			string y =line.substr(index+1,line.length());
			Point p(atoi(x.c_str()),atoi(y.c_str()));
			temp.push_back(p);
		}
	}
	else{
	    throw "�ļ�����ɾ";
	}
    file.close();
	return temp;

}

void FaceLogic::judgeFacePosture(const Point& lfeye,const Point& rteye,const Point& mouth)
{
    if(normal.size()==0)
    {
       normal = getNormalValue();
       if(normal.size()!=3)
           throw "�ļ�����";
    }

	/*if(lfeye.x < normal[0].x)
		rtType =Left;
	if(rteye.x > normal[1].x)
		rtType =Right;*/

	int distance1 =(lfeye.x - normal[0].x)*(lfeye.x - normal[0].x) +(lfeye.y -normal[0].y)*(lfeye.y -normal[0].y);
	int distance2 =(rteye.x - normal[1].x)*(rteye.x - normal[1].x) +(rteye.y -normal[1].y)*(rteye.y -normal[1].y);
	int distance3 =(mouth.x - normal[2].x)*(mouth.x - normal[2].x) +(mouth.y -normal[2].y)*(mouth.y -normal[2].y);

    //cout<<distance1<<"   "<<distance2<<"  "<<distance3;

	int dis_lr = (normal[0].x-normal[1].x)*(normal[0].x-normal[1].x) + (normal[0].y-normal[1].y)*(normal[0].y-normal[1].y);
	int dis_lm = (normal[0].x-normal[2].x)*(normal[0].x-normal[2].x) + (normal[0].y-normal[2].y)*(normal[0].y-normal[2].y);
	int dis_rm = (normal[1].x-normal[2].x)*(normal[1].x-normal[2].x) + (normal[1].y-normal[2].y)*(normal[1].y-normal[2].y);
	int dis_lr_realtime = (lfeye.x - rteye.x)*(lfeye.x - rteye.x)+(lfeye.y-rteye.y)*(lfeye.y-rteye.y);
	int dis_lm_realtime = (lfeye.x - mouth.x)*(lfeye.x - mouth.x) + (lfeye.y-mouth.y)*(lfeye.y-mouth.y);
	int dis_rm_realtime = (rteye.x-mouth.x)*(rteye.x-mouth.x) + (rteye.y-mouth.y)*(rteye.y-mouth.y);
	if(dis_rm==0||dis_lm==0||dis_lr==0)
	{
        rtType =Normal;
	}
	else if(dis_rm_realtime/dis_rm>1.2&&dis_lm_realtime/dis_lm>1.2&&dis_lr_realtime/dis_lr>1.2)
		{
            rtType =Near;
		}
		else if(dis_rm_realtime/dis_rm<0.8&&dis_lm_realtime/dis_lm<0.8&&dis_lr_realtime/dis_lr<0.8)
			{
                rtType =Far;
			}
			else
			{
                rtType =Normal;
			}
	if(distance1+distance2+distance3 > 24000)
	{
		if(abs(lfeye.x-normal[0].x)==0||abs(rteye.x-normal[1].x)==0||abs(mouth.x-normal[2].x)==0)
		{
			rtType = Normal;
		}
		else if(lfeye.x-normal[0].x>0&&rteye.x-normal[1].x>0&&mouth.x-normal[2].x>0)
		{
			if(abs(lfeye.y-normal[0].y)/abs(lfeye.x-normal[0].x)+abs(rteye.y-normal[1].y)/abs(rteye.x-normal[1].x)+abs(mouth.y-normal[2].y)/abs(mouth.x-normal[2].x) > 3)
			{
				if(lfeye.y-normal[0].y>0&&rteye.y-normal[1].y>0&&mouth.y-normal[2].y>0)
				{
					rtType = Down;
				}
				else
				{
					rtType = Up;
				}
			}
			else
			{
				rtType = Left;
			}
		}
		else if(abs(lfeye.y-normal[0].y)/abs(lfeye.x-normal[0].x)+abs(rteye.y-normal[1].y)/abs(rteye.x-normal[1].x)+abs(mouth.y-normal[2].y)/abs(mouth.x-normal[2].x) > 3)
		{
			if(lfeye.y-normal[0].y>0&&rteye.y-normal[1].y>0||mouth.y-normal[2].y>0)
			{
				rtType = Down;
			}
			else
			{
				rtType = Up;
			}
		}
		else
		{
			rtType = Right;
		}
	}
}


string FaceLogic::fetchJudgedMessage()
{

	switch (rtType)
	{
        case Normal:
            return "normal\n";
			break;
        case Profile:
            return "profile face\n";
			break;
        case Near:
            return "too close\n";
			break;
        case Far:
            return "too far\n";
			break;
        case Left:
            return "left\n";
			break;
        case Right:
            return "right\n";
			break;
        case Up:
            return "up\n";
			break;
        case Down:
            return "down\n";
			break;
        case NO_FACE:
            return "no face detected\n";
            break;
        case Fault_FACE:
            return "no entire face\n";
            break;
		default:
            return "normal\n";
			break;
    }
}

FacePostureType FaceLogic::getRtType()
{
    return rtType;
}

void FaceLogic::setRtType(FacePostureType rt)
{
    rtType =rt;
}