library(fields)
library(rjson)
library(pracma)
library(colorRamps)

makeSVG = F

rho = 1000
g = 9.81


data=fromJSON(file="input.json")
pdyn = matrix(data$pdyn, byrow=TRUE, nrow=data$nx, ncol=data$nz)
u = matrix(data$uorb, byrow=TRUE, nrow=data$nx, ncol=data$nz)
v = matrix(data$vorb, byrow=TRUE, nrow=data$nx, ncol=data$nz)
w = matrix(data$worb, byrow=TRUE, nrow=data$nx, ncol=data$nz)
z = repmat(t(data$z), n=data$nx, m=1)
ptot = pdyn + rho*g*z
depth = rep(data$depth, data$nx)
ncol = 16
colors = blue2green2red(ncol)#rev(heat.colors(ncol))


graphics.off()
if(makeSVG)
{
    svg("wave_colormap.svg",width=14,height=7)
}
par(mfrow=c(2,2),oma = c(0, 0, 2, 0))

image(data$x, data$z, pdyn, xlab="X", ylab="Z positive downwards [m]", col = colors, ylim = rev(range(data$z)))
image.plot(data$x, data$z, pdyn, legend.only=T, breaks=seq(from=min(pdyn),to=max(pdyn), length.out=ncol+1), col = colors)
grid()
lines(data$x, data$eta)
lines(data$x, depth, col='chartreuse4', lwd=4)
title('Dynamic pressure [N/m²]')


image(data$x, data$z, u, xlab="X", ylab="Z positive downwards [m]", col = colors, ylim = rev(range(data$z)))
image.plot(data$x, data$z, u, legend.only=T, breaks=seq(from=min(u),to=max(u), length.out=ncol+1), col = colors)
grid()
lines(data$x, data$eta)
lines(data$x, depth, col='chartreuse4', lwd=4)
title('Orbital velocity u [m/s]')

image(data$x, data$z, ptot, xlab="X", ylab="Z positive downwards [m]", col = colors, ylim = rev(range(data$z)))
image.plot(data$x, data$z, ptot, legend.only=T, breaks=seq(from=min(ptot),to=max(ptot), length.out=ncol+1), col = colors)
grid()
lines(data$x, data$eta)
lines(data$x, depth, col='chartreuse4', lwd=4)
title('Total pressure [N/m²]')

image(data$x, data$z, w, xlab="X", ylab="Z positive downwards [m]", col = colors, ylim = rev(range(data$z)))
image.plot(data$x, data$z, w, legend.only=T, breaks=seq(from=min(w),to=max(w), length.out=ncol+1), col = colors)
grid()
lines(data$x, data$eta)
lines(data$x, depth, col='chartreuse4', lwd=4)
title('Orbital velocity w [m/s]')

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

idx = 2*c(1:(length(data$x)/2)) # Less arrows (1 in two)
plot(data$x,data$eta, type='l', xlab="X [m]", ylab="Z positive downwards [m]", lwd=3, col='chartreuse4', asp=1, ylim = rev(range(data$eta)))
arrows(x0=data$x[idx],y0=data$eta[idx], x1=data$x[idx]+data$usurf[idx], y1=data$eta[idx]+data$wsurf[idx], length=0.1, lwd=2, col='blue')
grid()
title(paste("Orbital velocities for delta [-] = ", data$delta, ", h [m] = ", data$h, ", Hs [m] = ",data$Hs,", Tp [s] = ", data$Tp, ", psi [deg] = ", data$psi, sep=''))
if(makeSVG)
{
    dev.off()
}
