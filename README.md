# CoolQ C++ 骰娘

本仓库由 [CoolQ C++ SDK模板](https://github.com/cqmoe/cqcppsdk-template) 生成。

手动编译使用方法请参考 [CoolQ C++ SDK文档](https://cqcppsdk.cqp.moe/)。

编译前置要求：

· [Visual Studio 2019 / Visual Studio Code](https://visualstudio.microsoft.com/zh-hans/)
	
· [Vcpkg](https://github.com/Microsoft/vcpkg)
	
· [Boost](https://www.boost.org)

使用Vcpkg安装Boost库的方法参见["使用第三方库"](https://cqcppsdk.cqp.moe/guide/third-party-libs.html#vcpkg)和[帮助文档](https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=vs-2019), 安装完成后修改[CMakeSettings.json](https://github.com/Samarium150/cqcpp_dice_maid/blob/master/CMakeSettings.json#L73)中"environments"配置。