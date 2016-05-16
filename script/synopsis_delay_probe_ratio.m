% Read delay from synopsis for different probe ratios.
clear all; clc;

num_synopses = 10;
half_num_synopses = 5; % The number of synopsis from which we start to 
                       % record the statistics.
num_servers_str = '10000';
batch_size = 100;
arrival_rate_str = {'0.700000', '0.900000'};
total_time_str = '10000.000000';
probe_ratio_str_batch = {...
    '1.000000', '1.100000', '1.200000', '1.300000', '1.400000',...
    '1.500000', '1.600000', '1.700000', '1.800000', '1.900000',...
    '2.000000'};
data_dir = 'd1.0-2.0/';

num_probe_ratios = length(probe_ratio_str_batch);
% MIT uses constant probe ratio 2.
for I = 1:num_probe_ratios
    probe_ratio_str{1}{I} = '2.000000';
end
% List of probe ratios of BS and BSWF to plot against.
probe_ratio_str{2} = probe_ratio_str_batch;
probe_ratio_str{3} = probe_ratio_str_batch;
num_servers = str2double(num_servers_str);
batch_size_str = num2str(batch_size);
arrival_rate = str2num(char(arrival_rate_str)).';
for I = 1:3
    probe_ratio(I, 1:num_probe_ratios) =...
        str2num(char(probe_ratio_str{I})).';
end
probe_ratio_for_plotting = probe_ratio(3, :);
digits_after_decimal_point = 6;
total_time = str2num(char(total_time_str)).';
policy = {'mit', 'bs', 'bswf'};
policy_name = {'Pod', 'BS', 'BF'};
color = {'b-', 'gs--', 'rv-.'};
for I_rate = 1:length(arrival_rate)
    for I_policy = 1:length(policy)
        for I_probe_ratio = 1:size(probe_ratio, 2)
            total_time_str_plot = total_time_str;
            if I_policy == 1
                total_time_str_plot = total_time_str;
            end
            filename_suffix = ['synopsis_n', num_servers_str, '_b',...
                batch_size_str, '_a', arrival_rate_str{I_rate}, '_t',...
                total_time_str_plot, '_r',...
                probe_ratio_str{I_policy}{I_probe_ratio},'_',...
                policy{I_policy}, '_',...
                probe_ratio_str{I_policy}{I_probe_ratio}];
            disp(['Arrival rate is ', num2str(arrival_rate(I_rate))])
            disp(['Policy is ', policy{I_policy}])
            disp(['Probe ratio is ', num2str(probe_ratio(I_policy,...
                I_probe_ratio))])
            disp(['File suffix is ', filename_suffix])
            batch = dlmread([data_dir, 'batch_', filename_suffix]);
            batch_last_half = batch(num_synopses, :)...
                -batch(half_num_synopses, :);
            batch_delay(I_policy, I_probe_ratio) = batch_last_half(2)...
                /batch_last_half(1);
            task = dlmread([data_dir, 'task_', filename_suffix]);
            task_last_half = task(num_synopses, :)...
                -task(half_num_synopses, :);
            task_delay(I_policy, I_probe_ratio) = task_last_half(2)...
                /task_last_half(1);
        end
    end
    % Plot task delay.
    figure
    set(gca, 'FontSize', 24)
    hold on
    for I_policy = 1:length(policy)
        h = plot(probe_ratio_for_plotting, task_delay(I_policy, :),...
            color{I_policy}, 'LineWidth', 2, 'MarkerSize', 12);
        legend(h, policy_name{I_policy});
    end
    grid on
    xlabel('Probe ratio')
    ylabel('Average task delay')
    filename_infix = ['n', num2str(num_servers), '_b',...
        num2str(batch_size), '_a', num2str(arrival_rate(I_rate))];
    saveas(gcf, ['task_delays_variable_batch_size_', filename_infix,...
        '.fig'], 'fig')
    % Plot batch delay.
    figure
    set(gca, 'FontSize', 24)
    hold on
    for I_policy = 1:length(policy)
        h = plot(probe_ratio_for_plotting, batch_delay(I_policy, :),...
            color{I_policy}, 'LineWidth', 2, 'MarkerSize', 12);
        legend(h, policy_name{I_policy});
    end
    grid on
    xlabel('Probe ratio')
    ylabel('Average batch delay')
    saveas(gcf, ['batch_delays_variable_batch_size_', filename_infix,...
        '.fig'], 'fig')
end
