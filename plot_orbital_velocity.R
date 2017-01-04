library(fields)
library(rjson)
library(pracma)

makeSVG = T


data=fromJSON(file="input.json")
pdyn = matrix(data$pdyn, byrow=TRUE, nrow=data$nx, ncol=data$nz)
u = matrix(data$u, byrow=TRUE, nrow=data$nx, ncol=data$nz)
v = matrix(data$v, byrow=TRUE, nrow=data$nx, ncol=data$nz)
w = matrix(data$w, byrow=TRUE, nrow=data$nx, ncol=data$nz)
depth = rep(data$depth, data$nx)

if(makeSVG)
{
    svg("wave_colormap.svg",width=14,height=7)
}
par(mfrow=c(2,2),oma = c(0, 0, 2, 0))

image(data$x, data$z, pdyn, xlab="X", ylab="Z")
image.plot(data$x, data$z, pdyn, legend.only=T)
grid()
lines(data$x, data$eta)
lines(data$x, depth, col='chartreuse4', lwd=4)
title('Dynamic pressure (in N/m²)')


image(data$x, data$z, u, xlab="X", ylab="Z")
image.plot(data$x, data$z, u, legend.only=T)
grid()
lines(data$x, data$eta)
lines(data$x, depth, col='chartreuse4', lwd=4)
title('Orbital velocity u (in m/s)')

image(data$x, data$z, v, xlab="X", ylab="Z")
image.plot(data$x, data$z, v, legend.only=T)
grid()
lines(data$x, data$eta)
lines(data$x, depth, col='chartreuse4', lwd=4)
title('Orbital velocity v (in m/s)')

image(data$x, data$z, w, xlab="X", ylab="Z")
image.plot(data$x, data$z, w, legend.only=T)
grid()
lines(data$x, data$eta)
lines(data$x, depth, col='chartreuse4', lwd=4)
title('Orbital velocity w (in m/s)')

mtext(paste("Orb. vel. & dyn. press. for delta [-] = ", data$delta, ", h [m] = ", data$h, ", Hs [m] = ",data$Hs,", Tp [s] = ", data$Tp, ", psi [deg] = ", data$psi, sep=''), outer = TRUE, cex=2)
if(makeSVG)
{
    dev.off()
}

if(makeSVG)
{
    svg("orbital_velocities.svg",width=14,height=7)
} else
{
    x11()
}
plot(data$x[1:100],data$eta[1:100], type='l', xlab="X [m]", ylab="Z (down) [m]", lwd=3, col='chartreuse4')
quiver(x=data$x[1:100],y=data$eta[1:100], u=u[1:100,1], v=w[1:100,1], scale=0.1, lwd=2, col='blue')
grid()
title(paste("Orbital velocities for delta [-] = ", data$delta, ", h [m] = ", data$h, ", Hs [m] = ",data$Hs,", Tp [s] = ", data$Tp, ", psi [deg] = ", data$psi, sep=''))
if(makeSVG)
{
    dev.off()
}
