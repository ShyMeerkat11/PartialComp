clear
clc
xList = [];
yList = [];
L1 = 3.1875;
L2 = 3.1875;
for x = 0.0:0.06375:6.375
    for y = -3.1875:0.06375:3.1875
        t2_1_P1 = atan2d(x,y);
        sqrtPart = sqrt(x^2+y^2-((x^2+y^2)/(2*L2))^2);
        if isreal(sqrtPart)
            t2_1_P2 = -atan2d((x^2+y^2)/(2*L2), sqrtPart);
            t2_1 = t2_1_P1 + t2_1_P2;
            t1_1 = acosd((x-(L2*cosd(t2_1)))/L1);
            if (t2_1 >= 0 && t2_1 <=90) && (t1_1 >= 0 && t1_1 <=90)
                xList = [xList, x];
                yList = [yList, y];
            end
        end
    end
end
plot(xList,yList);


