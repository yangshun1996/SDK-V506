package com.fh;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import springfox.documentation.builders.ApiInfoBuilder;
import springfox.documentation.builders.PathSelectors;
import springfox.documentation.builders.RequestHandlerSelectors;
import springfox.documentation.service.ApiInfo;
import springfox.documentation.spi.DocumentationType;
import springfox.documentation.spring.web.plugins.Docket;
import springfox.documentation.swagger2.annotations.EnableSwagger2;

/**
 * @Title
 * @Author Lskj
 * @Date 2019-03-17 14:42
 * @Description http://localhost:6203/swagger-ui.html
 **/
@Configuration
@EnableSwagger2
public class Swagger2 {
    @Bean
    public Docket createRestApi() {
        return new Docket(DocumentationType.SWAGGER_2)
                .apiInfo(apiInfo())
                .select()
                //为当前包路径
                .apis(RequestHandlerSelectors.basePackage("com.fh.controller"))
                .paths(PathSelectors.any())
                .build();
    }
    //构建 api文档的详细信息函数,注意这里的注解引用的是哪个
    private ApiInfo apiInfo() {
        return new ApiInfoBuilder()
                //页面标题
                .title("camera-Server Controller")
                //版本号
                .version("1.0")
                //描述
                .description("camera。")
                .build();
    }
    /*
     @Api：修饰整个类，描述Controller的作用
     @ApiOperation：描述一个类的一个方法，或者说一个接口
     @ApiParam：单个参数描述
     @ApiModel：用对象来接收参数
     @ApiProperty：用对象接收参数时，描述对象的一个字段
     @ApiResponse：HTTP响应其中1个描述
     @ApiResponses：HTTP响应整体描述
     @ApiIgnore：使用该注解忽略这个API
     @ApiError ：发生错误返回的信息
     @ApiImplicitParam：一个请求参数
     @ApiImplicitParams：多个请求参数
     */
}
