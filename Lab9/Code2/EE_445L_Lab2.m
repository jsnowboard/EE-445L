fileID=fopen('array2.hex', 'r');
formatSpec = '%x';
sizeA = [2 Inf];
A = fscanf(fileID,formatSpec,sizeA)
fclose(fileID);
y=geocdf(A,0.5);
x=unique(A);
plot(x, histc(A(:), x));