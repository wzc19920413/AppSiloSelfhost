#include "pch.h"
#include "MainWindow.xaml.h"
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>
#include <microsoft.ui.xaml.window.h>
#include <Shobjidl.h>
#include <winrt/Windows.Storage.Pickers.h>
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage::Pickers;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::AppSiloSelfhost::implementation
{
    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
		ButtonClickHandler();
    }

	winrt::Windows::Foundation::IAsyncAction MainWindow::ButtonClickHandler()
	{
		try {
			FileOpenPicker picker;

			auto windowNative{ this->m_inner.as<::IWindowNative>() };
			HWND hWnd{ 0 };
			windowNative->get_WindowHandle(&hWnd);
			BOOL isValid = IsWindow(hWnd);

			auto initializeWithWindow{ picker.as<::IInitializeWithWindow>() };
			initializeWithWindow->Initialize(hWnd);

			picker.ViewMode(PickerViewMode::Thumbnail);
			picker.SuggestedStartLocation(PickerLocationId::PicturesLibrary);
			picker.FileTypeFilter().ReplaceAll({ L".txt" });
			StorageFile file = co_await picker.PickSingleFileAsync();

			// StorageFile file = co_await StorageFile::GetFileFromPathAsync(L"C:\\WorkloadPreference.txt");
			hstring text = co_await FileIO::ReadTextAsync(file);
			FileContentTextBlock().Text(text);
		}
		catch (hresult_error const& ex) {
			FileContentTextBlock().Text(L"Error: " + ex.message() + ex.code());
		}
	}
}
