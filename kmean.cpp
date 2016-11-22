#include "kmean.h"



int index_vector[18]={0, 205, 510, 868, 1185, 1534, 1804, 1892, 1971, 2307, 2596, 2909,
			3178, 3229, 3278, 3326, 3579, 3965};
string file_name[17]={"0_0512172825.bin", "1_0512171649.bin", "2_0512175502.bin",
            "3_0512173312.bin", "4_0512164800.bin", "5_0512164529.bin",
			"6_0512165243.bin", "7_0512165327.bin", "8_0512174513.bin",
			"9_0512174643.bin", "10_0512171207.bin", "11_0512171444.bin",
			"12_0512173520.bin", "13_0512173548.bin", "14_0512173623.bin",
			"15_0512170134.bin", "16_0512174930.bin"};
string file_out[17]={"0_0512172825.txt", "1_0512171649.txt", "2_0512175502.txt",
            "3_0512173312.txt", "4_0512164800.txt", "5_0512164529.txt",
			"6_0512165243.txt", "7_0512165327.txt", "8_0512174513.txt",
			"9_0512174643.txt", "10_0512171207.txt", "11_0512171444.txt",
			"12_0512173520.txt", "13_0512173548.txt", "14_0512173623.txt",
			"15_0512170134.txt", "16_0512174930.txt"};
float* getValueFromFile(unsigned idx)
{

    float *d = (float*)malloc(sizeof(float) * dim);
    float dd[dim];
    string filename;
    for(int i=1;i<18;i++)
    {
        if(idx<index_vector[i])
        {
            idx=idx-index_vector[i-1];
            filename = file_name[i-1];
            break;
        }
    }


    FILE *fin;
    fin=fopen(filename.c_str(),"rb");
    fseek(fin,idx*4*dim,SEEK_SET);
    //fread(d,sizeof(d)*dim,1,fin);
    fread(dd,sizeof(dd),1,fin);
    memcpy(d,dd,dim*4);
    //printf("%f %f %f \n",d[0],d[1],d[2]);
    fclose(fin);
    return d;
}
float randf(float m)
{
	return m * rand() / (RAND_MAX - 1.);
}

point load_point(int count)
{
	//allocate and init the array of points
	//double ang, r;
	point  pt = (point)malloc(sizeof(point_t) * count);

    //float p[dim];
    for(int i=0;i<count;i++)
    {
         //pt[i].idx=i*4*dim;//the index is the location in the file where we can load the value
         pt[i].idx=i;
         pt[i].group=-1;
    }





    /*FILE *fout;
    fout=fopen("group.bin","wb");

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0,1.0);



    for(int i=0;i<count;i++)
    {
		pt[i].idx=i*4*dim;

        p[0]=distribution(generator);
        p[1]=distribution(generator);
        p[2]=distribution(generator);

        fwrite(p,sizeof(p),1,fout);

    }

    fclose(fout);*/


	return pt;


}

inline double dist2(float* a, float* b, int len)
{

    double result=0;
	for(int i=0;i<len;i++)
	{
        result+=(a[i]-b[i])*(a[i]-b[i]);
  	}
  	return result;
}

inline int
nearest(point pt, point_center cent, int n_cluster, double *d2)
{
	int i, min_i;
	point_center c;
	double d, min_d;

    float *cc = (float*)malloc(sizeof(float) * dim);

#	define for_n for (c = cent, i = 0; i < n_cluster; i++, c++)
	//for_n
	{
		min_d = HUGE_VAL;
		min_i = pt->group;
		for_n {

            //std::copy(std::begin(c.v), std::end(c.v), std::begin(cc));
            memcpy(cc,c->v,dim*4);
            float* ptt=getValueFromFile(pt->idx);
            d=dist2(cc,ptt,dim);
            free(ptt);
			if (min_d > d/*(d = dist2(c, pt))*/) {
				min_d = d; min_i = i;
			}
		}
	}
	if (d2)
	{
	*d2 = min_d;
	}
    free(cc);
	return min_i;
}

void kpp(point pts, int len, point_center cent, int n_cent)
{
#	define for_len for (j = 0, p = pts; j < len; j++, p++)
	int i, j;
	int n_cluster;
	double sum, *d = (double*)malloc(sizeof(double) * len);

	point p;
	point_center c;
	//cent[0] = pts[ rand() % len ];
	//to be implement : load pts[rand()%len] from file, copy it to value of v

    cent[0].group=pts[ rand() % len ].group;
    float* tem=getValueFromFile(pts[ rand() % len ].idx);
    //std::copy(std::begin(tem), std::end(tem), std::begin(cent[0].v));
    memcpy(cent[0].v,tem,dim*4);
    free(tem);

	for (n_cluster = 1; n_cluster < n_cent; n_cluster++)
	{
        sum = 0;
		for_len {
			nearest(p, cent, n_cluster, d + j);
			sum += d[j];
		}
		sum = randf(sum);
		for_len
		{
			if ((sum -= d[j]) > 0) continue;
			//cent[n_cluster] = pts[j];
               cent[n_cluster].group=pts[j].group;
               tem=getValueFromFile(pts[j].idx);
                //std::copy(std::begin(tem), std::end(tem), std::begin(cent[n_cluster].v));
                memcpy(cent[n_cluster].v,tem,dim*4);
                free(tem);
                printf("center of group %d = %d OK\n",n_cluster,j);

			break;
		}

	}
	for_len p->group = nearest(p, cent, n_cluster, 0);
	free(d);
}

point_center lloyd(point pts, int len, int n_cluster)
{
	int i, j, min_i;
	int changed;

	point_center cent = (point_center)malloc(sizeof(point_tc) * n_cluster);
	point p;
	point_center c;

	/* assign init grouping randomly */
	//for_len p->group = j % n_cluster;

	/* or call k++ init */
	kpp(pts, len, cent, n_cluster);
    printf("Initialization :::DONE   Starting the process \n");
	do {
		/* group element for centroids are used as counters */
		for_n {
            c->group = 0;
            //c->x = c->y = 0;
             for(int iii=0;iii<dim;iii++)
             {
                c->v[iii]=0;
             }
		 }
		for_len {
			c = cent + p->group;
			c->group++;
			//c->x += p->x; c->y += p->y;
             float *tem=getValueFromFile(p->idx);
             for(int iii=0;iii<dim;iii++)
             {
                c->v[iii]+=tem[iii];
             }
             free(tem);
		}
		for_n { /*c->x /= c->group; c->y /= c->group;*/
             for(int iii=0;iii<dim;iii++)
             {
                c->v[iii]/=c->group;
             }
		 }

		changed = 0;
		/* find closest centroid of each point */
		for_len {
			min_i = nearest(p, cent, n_cluster, 0);
			if (min_i != p->group) {
				changed++;
				p->group = min_i;
			}
		}
		printf("number of changes= %d \n",changed);
	} while (changed > (len >> 10)); /* stop when 99.9% of points are good */

	for_n { c->group = i; }

	return cent;
}

#	undef for_n
#	undef for_len

int kmean()
{
	int i;
	point v = load_point(PTS);
    for(int ii=0;ii<PTS;ii++)
    {
        float* r=getValueFromFile(v[ii].idx);

        printf("%f  %f  %f\n",r[0],r[1],r[2]);

        free(r);

    }


	point_center c = lloyd(v, PTS, K);
	ofstream out;
	out.open("center.txt", ofstream::out);

	//print_eps(v, PTS, c, K);
    int cluster[K]={0,0,0,0,0,0,0,0,0,0,0};
    for(int i=0;i<PTS;i++)
    {
        cluster[v[i].group]++;
    }
    for(int i=0;i<11;i++)
	{
        out << "group: " << c[i].group << endl;
        cout << "group: " << c[i].group << endl;
        out << c[i].v[0] << " " << c[i].v[1] << " " << c[i].v[2] << " " << c[i].v[dim-2] << " " << c[i].v[dim-1] << endl;
        cout << c[i].v[0] << " " << c[i].v[1] << " " << c[i].v[2] << " " << c[i].v[dim-2] << " " << c[i].v[dim-1] << endl;
        printf("group %d, number of element %d \n",i,cluster[i]);
	}
	out.close();
    cout << "----------------------------------------" << endl;

	for(int i=0;i<PTS;i++)
	{
        float* r = getValueFromFile(v[i].idx);
        double total = 0;
        float *tem = (float*)malloc(sizeof(float) * dim);
        memcpy(tem,c[0].v,dim*4);

        double min_distance = dist2(r, tem, dim);
        double distance;
        total += min_distance;
        for(int j=1;j<11;++j)
        {
           memcpy(tem,c[j].v,dim*4);
            distance = dist2(r, tem, dim);

            total += distance;
            if(distance<min_distance)
                min_distance = distance;
        }
        free(r);
        free(tem);
        double p = min_distance/total;
        double entropy = -p * log2(p);
        string filename;
        for(int j=1;j<18;j++)
        {
            if(i<index_vector[j])
            {
                filename = file_out[j-1];
                break;
            }
        }
        out.open(filename, ofstream::app);
        out << i << " " << v[i].group << " " << entropy << endl;
        out.close();
	}

    free(v); free(c);


	return 0;
}
