using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Data;

namespace VisualizationPlayer
{
    class BooleanToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, string language)
        {
            Visibility visibility = (Visibility)value;
            return parameter?.ToString() == "!" ? visibility == Visibility.Collapsed : visibility == Visibility.Visible;
        }

        public object ConvertBack(object value, Type targetType, object parameter, string language)
        {
            bool bValue = (bool)value;
            if (parameter?.ToString() == "!")
                bValue = !bValue;
            return bValue ? Visibility.Visible : Visibility.Collapsed;
        }
    }
}
