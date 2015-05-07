var smoothieUVW = new SmoothieChart({maxValueScale:10.0,minValueScale:-10.0, labels:{disabled:true}});
smoothieUVW.streamTo(document.getElementById("canvasUVW"), 1000 /*delay*/);

var smoothiePQR = new SmoothieChart({maxValueScale:10.0,minValueScale:-10.0, labels:{disabled:true}});
smoothiePQR.streamTo(document.getElementById("canvasPQR"), 1000 /*delay*/);

// UVW
var lineU = new TimeSeries();
var lineV = new TimeSeries();
var lineW = new TimeSeries();
smoothieUVW.addTimeSeries(lineU, {lineWidth:2,strokeStyle:'#ff0000'});
smoothieUVW.addTimeSeries(lineV, {lineWidth:2,strokeStyle:'#00ff00'});
smoothieUVW.addTimeSeries(lineW, {lineWidth:2,strokeStyle:'#0000ff'});

// PQR
var lineP = new TimeSeries();
var lineQ = new TimeSeries();
var lineR = new TimeSeries();
smoothiePQR.addTimeSeries(lineP, {lineWidth:2,strokeStyle:'#ff0000'});
smoothiePQR.addTimeSeries(lineQ, {lineWidth:2,strokeStyle:'#00ff00'});
smoothiePQR.addTimeSeries(lineR, {lineWidth:2,strokeStyle:'#0000ff'});

function appendSmoothie(u,v,w,p,q,r) {
    u = u || 0.0;
    v = v || 0.0;
    w = w || 0.0;
    p = p || 0.0;
    q = q || 0.0;
    r = r || 0.0;
    var t = new Date().getTime();
    lineU.append(t, u);
    lineV.append(t, v);
    lineW.append(t, w);
    lineP.append(t, p);
    lineQ.append(t, q);
    lineR.append(t, r);
}
