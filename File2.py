import shape

reload(shape)
from shape import Shapes

#reload(shape)

rectangle = Shapes(1,2)
print rectangle.area()