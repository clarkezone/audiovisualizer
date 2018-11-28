# IBarElementFactory
To create custom shaped elements for DiscreteVUBar and SpectrumVisualizer you need to create a class that implements this interface and set ElementFactory property of the control to this class

## Methods
### CreateElementBrush
#### Arguments
##### sender (Object)
The control calling this method
##### elementColor (Windows.UI.Color)
Provides the color that this element should be rendered in
##### size (Windows.Foundation.Size)
The size of the element cell
##### compositor (Windows.UI.Composition.Compositor)
Compositor object
##### device (Windows.UI.Composition.CompositionGraphicsDevice)
Composition device
#### Return Value
Implemented method needs to return Window.UI.Composition.CompositionBrush object that will be used to render the element

### CreateShadowMask
#### Arguments
##### sender (Object)
The control calling this method
##### size (Windows.Foundation.Size)
The size of the element cell
##### compositor (Windows.UI.Composition.Compositor)
Compositor object
##### device (Windows.UI.Composition.CompositionGraphicsDevice)
Composition device
#### Return Value
Implemented method needs to return Window.UI.Composition.CompositionBrush object that will be used for the element shadow mask.

### Sample
This sample creates circular elements and circular shadow masks
```C#
public class CircularElementFactory : IBarElementFactory
{
    public CompositionBrush CreateElementBrush(object sender, Color     elementColor, Size size, Compositor compositor,     CompositionGraphicsDevice device)
    {
        if (size.Width == 0 && size.Height == 0)
                    return null;
        var surface = device.CreateDrawingSurface(size,     DirectXPixelFormat.B8G8R8A8UIntNormalized,  DirectXAlphaMode.Premultiplied);
        using (var drawingSession =     CanvasComposition.CreateDrawingSession(surface))
        {
            drawingSession.Clear(Colors.Transparent);
            var center = size.ToVector2() / 2.0f;
            var radius = center.X > center.Y ? center.Y : center.X;
            drawingSession.FillCircle(center,radius, elementColor);
        }
        return compositor.CreateSurfaceBrush(surface);
    }
    
    public CompositionBrush CreateShadowMask(object sender, Size size,  Compositor compositor, CompositionGraphicsDevice device)
    {
        if (size.Width == 0 && size.Height == 0)
            return null;
    
        var surface = device.CreateDrawingSurface(size,     DirectXPixelFormat.B8G8R8A8UIntNormalized,  DirectXAlphaMode.Premultiplied);
        using (var drawingSession =     CanvasComposition.CreateDrawingSession(surface))
        {
            drawingSession.Clear(Colors.Transparent);
            var center = size.ToVector2() / 2.0f;
            var radius = center.X > center.Y ? center.Y : center.X;
            drawingSession.FillCircle(center, radius, Color.FromArgb    (255,255,255,255));
        }
        return compositor.CreateSurfaceBrush(surface);
    }
}
```

[Home](AudioVisualizer.md)