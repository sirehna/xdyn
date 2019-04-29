function data = tbx_io_stlRead(file, option)
% TBX_IO_STLREAD reads ascii and binary STL files.
%
% data = tbx_io_stlRead(file);
% data = tbx_io_stlRead(file, option);
%
% % Example of use 1
% figure
% data = tbx_io_stlRead(file);
% % patch('Faces',data.faces,'Vertices',data.vertices,'FaceColor','g');
% patch('Faces',data.faces,'Vertices',data.vertices,'FaceColor','g','EdgeColor','none');
% axis equal
%
% % Example of use 2
% figure
% for i=0:39
%     data = tbx_io_stlRead(sprintf('split%03d.stl',i));
%     patch('Faces',data.faces, 'Vertices',data.vertices,...
%           'FaceColor',rand(1,3), 'EdgeColor','none');
% end
% axis equal
%
% This function relies on 3 functions found on mathworks, with BSD/MIT like
% license. Licences are provided at the beginning of each function
%
% [f,v,c]=rndread from cad2matdemo Don Riley Jun 2003 : read ASCII STL with color
%   <http://www.mathworks.com/matlabcentral/fileexchange/3642-cad2matdemo-m>
%
% [v,f,n]=import_stl_fast(file, 1) Eric Trautmann 31 Mar 2011 : read ASCII fast, no color
%  <http://www.mathworks.com/matlabcentral/fileexchange/30923-fast-stl-import-function>
%
% stlread(file) Francis Esmonde-White, May 2010 : read STL binary
%  <http://www.mathworks.com/matlabcentral/fileexchange/29906-binary-stl-file-reader>
%  [v, f, n, c, stltitle] = stlread('MyModel.stl');
%
%
% Copyright (c) 2003, Don Riley
% All rights reserved.
% 
% Redistribution and use in source and binary forms, with or without
% modification, are permitted provided that the following conditions are met:
% 
% * Redistributions of source code must retain the above copyright notice, this
%   list of conditions and the following disclaimer.
% 
% * Redistributions in binary form must reproduce the above copyright notice,
%   this list of conditions and the following disclaimer in the documentation
%   and/or other materials provided with the distribution
% * Neither the name of Walla Walla University nor the names of its
%   contributors may be used to endorse or promote products derived from this
%   software without specific prior written permission.
% THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
% AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
% IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
% DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
% FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
% DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
% SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
% CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
% OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
% OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

data=[];
if isempty(file), return; end
if nargin == 1, option=''; end
if isempty(option)
  % try all importers
  try
    data = tbx_io_stlRead(file, 'ascii');
  end
  if isempty(data),
    data = tbx_io_stlRead(file, 'binary');
  end
  return;
end

v=[]; f=[]; n=[]; c=[]; stltitle='';
% binary format
if strncmp(option, 'bin',3)
  [v, f, n, c, stltitle] = stlread(file);
else  % ascii, text
  % first tries the fast method
  try
    [v,f,n]=import_stl_fast(file, 1);
  end

  % then the slower one
  if isempty(n) || isempty(v)
    [f,v,c]=rndread(file);
  end
end

if isempty(v), return; end

data.faces   = f;
data.vertices= v;
if ~isempty(c)
  data.FaceVertexCData=c;
end
if ~isempty(strtrim(stltitle))
  data.Title=[ file ' ' strtrim(stltitle) ];
end
if ~isempty(n)
  data.Normals=n;
end

% ------------------------------------------------------------------------------
function varargout=import_stl_fast(filename,mode)
% function to import ASCII STL files into matlab.  This update uses the
% textscan function instead of reading the file line by line, which can
% significantly reduce import times for very large STL files.
%--------------------------------------------------------------
%
% inputs:       filename:   string
%               mode:       integer 1 or 2, see outputs:
%
% outputs:      mode 1: [points,triangles,tri norms]
%               mode 2: [vertices, tri norms]
%
% author: Eric Trautmann, 3/31/2011
%         etrautmann@gmail.com
%         based on code by Luigi Giaccari.
%         future revisions will include support for binary files
%-----------------------------------------------------------------
% Ex:
% filename = 'file.stl'
% [p,t,tnorm] = import_stl_fast(filename,1)

%  Copyright (c) 2011, Eric Trautmann
%  All rights reserved.
%
%  Redistribution and use in source and binary forms, with or without
%  modification, are permitted provided that the following conditions are
%  met:
%
%      * Redistributions of source code must retain the above copyright
%        notice, this list of conditions and the following disclaimer.
%      * Redistributions in binary form must reproduce the above copyright
%        notice, this list of conditions and the following disclaimer in
%        the documentation and/or other materials provided with the distribution
%
%  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
%  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
%  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
%  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
%  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
%  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
%  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
%  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
%  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
%  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
%  POSSIBILITY OF SUCH DAMAGE.

if nargin<2
    mode=1;%default value
end

if ~(mode==1 || mode==2)
    error('invalid mode')
end

if nargout<3 && mode==1
    error('invalid input number /mode setting')
end
if nargout>2 && mode==2
    error('invalid input number /mode setting')
end


%=====================================================
%open file
fid=fopen(filename, 'r'); %Open the file, assumes STL ASCII format.
if fid == -1
    error('File could not be opened, check name or path.')
end

fmt = '%*s %*s %f32 %f32 %f32 \r\n %*s %*s \r\n %*s %f32 %f32 %f32 \r\n %*s %f32 %f32 %f32 \r\n %*s %f32 %f32 %f32 \r\n %*s \r\n %*s \r\n';
C=textscan(fid, fmt, 'HeaderLines', 1);
fclose(fid);
if isempty(C), varargout{1}=[]; return; end

%extract normal vectors and vertices
tnorm = cell2mat(C(1:3));
tnorm = tnorm(1:end-1,:); %strip off junk from last line

v1 = cell2mat(C(4:6));
v2 = cell2mat(C(7:9));
v3 = cell2mat(C(10:12));

if (C{4}(end)== NaN)
    v1 = v1(1:end-1,:); %strip off junk from last line
    v2 = v2(1:end-1,:); %strip off junk from last line
    v3 = v3(1:end-1,:); %strip off junk from last line
end

v_temp = [v1 v2 v3]';
v = zeros(3,numel(v_temp)/3);

v(:) = v_temp(:);
v = v';

varargout = cell(1,nargout);
switch mode
    case 1
        [p,t]=fv2pt(v,length(v)/3);%gets points and triangles

        varargout{1} = p;
        varargout{2} = t;
        varargout{3} = tnorm;
    case 2
        varargout{1} = v;
        varargout{2} = tnorm;
end


%%
function [p,t]=fv2pt(v,fnum)

%gets points and triangle indexes given vertex and facet number
c=size(v,1);

%triangles with vertex id data
t=zeros(3,fnum);
t(:)=1:c;

%now we have to keep unique points fro vertex
[p,i,j]=unique(v,'rows'); %now v=p(j) p(i)=v;
t(:)=j(t(:));
t=t';

% ------------------------------------------------------------------------------

function [fout, vout, cout] = rndread(filename)
% Reads CAD STL ASCII files, which most CAD programs can export.
% Used to create Matlab patches of CAD 3D data.
% Returns a vertex list and face list, for Matlab patch command.
%
% filename = 'hook.stl';  % Example file.
%

%  Copyright (c) 2003, Don Riley
%  All rights reserved.
%
%  Redistribution and use in source and binary forms, with or without
%  modification, are permitted provided that the following conditions are
%  met:
%
%      * Redistributions of source code must retain the above copyright
%        notice, this list of conditions and the following disclaimer.
%      * Redistributions in binary form must reproduce the above copyright
%        notice, this list of conditions and the following disclaimer in
%        the documentation and/or other materials provided with the distribution
%
%  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
%  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
%  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
%  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
%  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
%  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
%  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
%  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
%  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
%  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
%  POSSIBILITY OF SUCH DAMAGE.

fid=fopen(filename, 'r'); %Open the file, assumes STL ASCII format.
if fid == -1
    error('File could not be opened, check name or path.')
end
%
% Render files take the form:
%
%solid BLOCK
%  color 1.000 1.000 1.000
%  facet
%      normal 0.000000e+00 0.000000e+00 -1.000000e+00
%      normal 0.000000e+00 0.000000e+00 -1.000000e+00
%      normal 0.000000e+00 0.000000e+00 -1.000000e+00
%    outer loop
%      vertex 5.000000e-01 -5.000000e-01 -5.000000e-01
%      vertex -5.000000e-01 -5.000000e-01 -5.000000e-01
%      vertex -5.000000e-01 5.000000e-01 -5.000000e-01
%    endloop
% endfacet
%
% The first line is object name, then comes multiple facet and vertex lines.
% A color specifier is next, followed by those faces of that color, until
% next color line.
%
CAD_object_name = sscanf(fgetl(fid), '%*s %s');  %CAD object name, if needed.
%                                                %Some STLs have it, some don't.
vnum=0;       %Vertex number counter.
VColor = 0;
%
while feof(fid) == 0                    % test for end of file, if not then do stuff
    tline = fgetl(fid);                 % reads a line of data from file.
    fword = sscanf(tline, '%s ');       % make the line a character string
% Check for color
    if strncmpi(fword, 'c',1) == 1;    % Checking if a 'C'olor line, as 'C' is 1st char.
       VColor = sscanf(tline, '%*s %f %f %f'); % & if a C, get the RGB color data of the face.
    end                                % Keep this color, until the next color is used.
    if strncmpi(fword, 'v',1) == 1;    % Checking if a 'V'ertex line, as 'V' is 1st char.
       vnum = vnum + 1;                % If a V we count the # of V's

       v(:,vnum) = sscanf(tline, '%*s %f %f %f'); % & if a V, get the XYZ data of it.
       c(:,vnum) = VColor;              % A color for each vertex, which will color the faces.
    end                                 % we '*s' skip the name 'color' and get the data.
end
%   Build face list; The vertices are in order, so just number them.
%
fnum = vnum/3;      %Number of faces, vnum is number of vertices.  STL is triangles.
flist = 1:vnum;     %Face list of vertices, all in order.
F = reshape(flist, 3,fnum); %Make a '3 by fnum' matrix of face list data.
%
%   Return the faces and vertexs.
%
fout = F';  %Orients the array for direct use in patch.
vout = v';  %
cout = c';
%
fclose(fid);

% ------------------------------------------------------------------------------
function [v, f, n, c, stltitle] = stlread(filename, verbose)
% This function reads an STL file in binary format into vertex and face
% matrices v and f.
%
% USAGE: [v, f, n, c, stltitle] = stlread(filename, verbose);
%
% verbose is an optional logical argument for displaying some loading
%   information (default is false).
%
% v contains the vertices for all triangles [3*n x 3].
% f contains the vertex lists defining each triangle face [n x 3].
% n contains the normals for each triangle face [n x 3].
% c is optional and contains color rgb data in 5 bits [n x 3].
% stltitle contains the title of the specified stl file [1 x 80].
%
% To see plot the 3D surface use:
%   patch('Faces',f,'Vertices',v,'FaceVertexCData',c);
% or
%   plot3(v(:,1),v(:,2),v(:,3),'.');
%
% Duplicate vertices can be removed using:
%   [v, f]=patchslim(v, f);
%
% For more information see:
%  http://www.esmonde-white.com/home/diversions/matlab-program-for-loading-stl-files
%
% Based on code originally written by:
%    Doron Harlev
% and combined with some code by:
%    Eric C. Johnson, 11-Dec-2008
%    Copyright 1999-2008 The MathWorks, Inc.
%
% Re-written and optimized by Francis Esmonde-White, May 2010.

%  Copyright (c) 2011, Francis Esmonde-White
%  Copyright (c) 2011, Eric Johnson
%  All rights reserved.
%
%  Redistribution and use in source and binary forms, with or without
%  modification, are permitted provided that the following conditions are
%  met:
%
%      * Redistributions of source code must retain the above copyright
%        notice, this list of conditions and the following disclaimer.
%      * Redistributions in binary form must reproduce the above copyright
%        notice, this list of conditions and the following disclaimer in
%        the documentation and/or other materials provided with the distribution
%
%  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
%  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
%  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
%  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
%  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
%  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
%  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
%  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
%  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
%  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
%  POSSIBILITY OF SUCH DAMAGE.

use_color=(nargout>=4);

fid=fopen(filename, 'r'); %Open the file, assumes STL Binary format.
if fid == -1
    error('File could not be opened, check name or path.')
end

if ~exist('verbose','var')
    verbose = false;
end

ftitle=fread(fid,80,'uchar=>schar'); % Read file title
numFaces=fread(fid,1,'int32'); % Read number of Faces

T = fread(fid,inf,'uint8=>uint8'); % read the remaining values
fclose(fid);

stltitle = char(ftitle');

if verbose
    fprintf('\nTitle: %s\n', stltitle);
    fprintf('Number of Faces: %d\n', numFaces);
    disp('Please wait...');
end

% Each facet is 50 bytes
%  - Three single precision values specifying the face normal vector
%  - Three single precision values specifying the first vertex (XYZ)
%  - Three single precision values specifying the second vertex (XYZ)
%  - Three single precision values specifying the third vertex (XYZ)
%  - Two color bytes (possibly zeroed)

% 3 dimensions x 4 bytes x 4 vertices = 48 bytes for triangle vertices
% 2 bytes = color (if color is specified)

trilist = 1:48;

ind = reshape(repmat(50*(0:(numFaces-1)),[48,1]),[1,48*numFaces])+repmat(trilist,[1,numFaces]);
Tri = reshape(typecast(T(ind),'single'),[3,4,numFaces]);

n=squeeze(Tri(:,1,:))';
n=double(n);

v=Tri(:,2:4,:);
v = reshape(v,[3,3*numFaces]);
v = double(v)';

f = reshape(1:3*numFaces,[3,numFaces])';

if use_color
    c0 = typecast(T(49:50),'uint16');
    if (bitget(c0(1),16)==1)
        trilist = 49:50;
        ind = reshape(repmat(50*(0:(numFaces-1)),[2,1]),[1,2*numFaces])+repmat(trilist,[1,numFaces]);
        c0 = reshape(typecast(T(ind),'uint16'),[1,numFaces]);
        r=bitshift(bitand(2^16-1, c0),-10);
        g=bitshift(bitand(2^11-1, c0),-5);
        b=bitand(2^6-1, c0);
        c=[r; g; b]';
    else
        c = zeros(numFaces,3);
    end
end

if verbose
    disp('Done!');
end
