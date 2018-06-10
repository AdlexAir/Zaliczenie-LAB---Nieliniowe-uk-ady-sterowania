x1=0;
y1=0;

x2=0;
y2=21.5;

x3=18.6;
y3=y2/2;

r=22;
xc=10.75;
yc=9.3;
zc=30;

wozek1=zc+sqrt(r^2-(x1-xc)^2-(y1-yc)^2);
wozek2=zc+sqrt(r^2-(x2-xc)^2-(y2-yc)^2);
wozek3=zc+sqrt(r^2-(x3-xc)^2-(y3-yc)^2);

ramie1=line([x1 xc],[y1 yc],[wozek1 zc]);
ramie2=line([x2 xc],[y2 yc],[wozek2 zc]);
ramie3=line([x3 xc],[y3 yc],[wozek3 zc]);

Setpoint = 80;

if ~exist('s','var')
    s = serial('/dev/ttyUSB0');
    s.BaudRate = 57600;
end

if s.Status(1)=='c'
    fopen(s);
end
pause(3)

Input = [32000;32000;32000;Setpoint];
fprintf(s,'%d %d %d %d\n',Input);
while(s.BytesAvailable == 0)
end
while(s.BytesAvailable > 0)
    rData = fscanf(s)
end



Input = [-2000;-2000;-2000;Setpoint];
fprintf(s,'%d %d %d %d\n',Input);
pause(3)
while(s.BytesAvailable == 0)
end

while(s.BytesAvailable > 0)
    rData = fscanf(s)
end
pause(1)
while(rData(1) == "N")
    Input = [0;0;0;Setpoint];
    fprintf(s,'%d %d %d %d\n',Input);
    while(s.BytesAvailable > 0)
    rData = fscanf(s)
    end
end
pause(1)


for t=0:0.05:2*pi
   xc=10.75 + 2*sin(t);
   yc=6.3+2*cos(t);
   zc=50;
   
   wozek1=zc+sqrt(r^2-(x1-xc)^2-(y1-yc)^2);
   wozek2=zc+sqrt(r^2-(x2-xc)^2-(y2-yc)^2);
   wozek3=zc+sqrt(r^2-(x3-xc)^2-(y3-yc)^2);
   
   
   
   %% do wozka
   if(t>0)
    krok1=round(200*(wozek1-stare1));
    krok2=round(200*(wozek2-stare2));
    krok3=round(200*(wozek3-stare3));

    Input = [krok1;krok2;krok3;Setpoint]
    fprintf(s,'%d %d %d %d\n',Input);
    while(s.BytesAvailable == 0)
    end
    %pause(0.001);

   while(s.BytesAvailable > 0)
    rData = fscanf(s);
    end
   end
   
   stare1= wozek1;
   stare2=wozek2;
   stare3=wozek3;
   
end