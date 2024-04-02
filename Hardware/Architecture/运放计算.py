r1 = 1000
r2 = 20000
r3 = 20000
r4 = 1000
Vref = 1.65
I = 0
R = 0.005
IU = I*R
ADC = (r1+r2)/(r3+r4)*(r3/r1)*Vref-(r4/r1)*(r1+r2)/(r3+r4)*IU
for times in range(2000):
    I = -times*0.01
    IU = I*R
    ADC = (r1+r2)/(r3+r4)*(r4/r1)*Vref-(r2/r1)*IU
    print(ADC, I)
    if ADC >= 3.3:
        break
