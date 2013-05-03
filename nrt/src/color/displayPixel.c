/*******************
	displayPixel.c
*******************/
void displayPixel(int row,int column, RGBColor_t rawColor)
{
	if(vp.showOutOfGamut)
	{
		//clamp To Color
		//mappedColor = clampToColor(rawColor);
		if(rawColor.r > 1.0 || rawColor.g > 1.0 || rawColor.b > 1.0)
		{
			rawColor.r = 1.0; 
			rawColor.g = 0.0; 
			rawColor.b = 0.0;
		}
	}
	else
	{
		//max to one
		//mappedColor = maxToOne(rawColor);
		FLOAT_T maxValue = max(rawColor.r,max(rawColor.g,rawColor.b));
		if(maxValue > 1.0)
		{
			rawColor.r = rawColor.r / maxValue;
			rawColor.g = rawColor.g / maxValue;
			rawColor.b = rawColor.b / maxValue;
		}
	}
	if(vp.gamma != 1.0)
	{
		//mappedColor = mappedColor.powc(vp.invGamma);
		//RGBColor( pow(r,p), pow(g,p), pow(b,p) )
		rawColor.r = pow(rawColor.r,vp.invGamma);
		rawColor.g = pow(rawColor.g,vp.invGamma);
		rawColor.b = pow(rawColor.b,vp.invGamma);
	}
	//Have to star from max y coordinate to convert to screen coordinates
	int x = column;
	//int y = vp.vres - row -1;
	int y = row;
	//printf("[x,y: %d %d ] color: [%f , %f ,%f] \n",x,y,rawColor.r*255,rawColor.g*255,rawColor.b*255);
	frame[x][y][0] = (int)(rawColor.r*255); 
	frame[x][y][1] = (int)(rawColor.g*255);
	frame[x][y][2] = (int)(rawColor.b*255);
	//printf("x,y: %d %d  color: %d , %d ,%d \n",x,y,frame[x][y][0],frame[x][y][1],frame[x][y][2]);
}